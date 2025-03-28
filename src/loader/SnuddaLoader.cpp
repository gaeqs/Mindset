//
// Created by gaeqs on 14/03/25.
//

#include <mindset/DefaultProperties.h>
#include <mindset/loader/SnuddaLoader.h>
#include <mindset/loader/SWCLoader.h>
#include <mindset/util/NeuronTransform.h>
#include <mindset/util/MorphologyUtils.h>
#include <rush/matrix/mat.h>
#include <rush/vector/vec.h>

namespace
{
    constexpr float METER_MICROMETER_RATIO = 1000000.0f;
}

namespace mindset
{

    SnuddaLoaderProperties SnuddaLoader::initProperties(Properties& properties) const
    {
        SnuddaLoaderProperties result{};
        if (_loadMorphology) {
            result.neuritePosition = properties.defineProperty(PROPERTY_POSITION);
            result.neuriteRadius = properties.defineProperty(PROPERTY_RADIUS);
            result.neuriteParent = properties.defineProperty(PROPERTY_PARENT);
            result.neuriteType = properties.defineProperty(PROPERTY_NEURITE_TYPE);
        }

        if (_loadSynapses) {
            result.synapsePreSection = properties.defineProperty(PROPERTY_SYNAPSE_PRE_SECTION);
            result.synapsePostSection = properties.defineProperty(PROPERTY_SYNAPSE_POST_SECTION);
        }

        result.neuronTransform = properties.defineProperty(PROPERTY_TRANSFORM);
        return result;
    }

    void SnuddaLoader::loadNeurons(Dataset& dataset, const SnuddaLoaderProperties& properties) const
    {
        auto ids = _file.getDataSet("network/neurons/neuron_id").read<std::vector<uint32_t>>();
        auto position = _file.getDataSet("network/neurons/position").read<std::vector<std::array<double, 3>>>();
        auto rotation = _file.getDataSet("network/neurons/rotation").read<std::vector<std::array<double, 9>>>();
        auto origo = _file.getDataSet("meta/simulation_origo").read<std::array<double, 3>>();

        auto origin = rush::Vec3f(origo[0], origo[1], origo[2]) * METER_MICROMETER_RATIO;

        for (size_t i = 0; i < ids.size(); ++i) {
            rush::Vec3f pos(position[i][0], position[i][1], position[i][2]); // Position in meters
            rush::Mat3f rot([array = rotation[i]](size_t c, size_t r) { return static_cast<float>(array[c + r * 3]); });

            rush::Mat4f model(rot, 1.0f);
            model[3] = rush::Vec4f(pos * METER_MICROMETER_RATIO + origin, 1.0f);

            Neuron neuron(ids[i]);
            neuron.setProperty(properties.neuronTransform, NeuronTransform(model));
            dataset.addNeuron(std::move(neuron));
        }
    }

    std::optional<std::string> SnuddaLoader::loadMorphologies(Dataset& dataset) const
    {
        static const std::string SNUDDA_PREFIX = "$SNUDDA_DATA";

        auto ids = _file.getDataSet("network/neurons/neuron_id").read<std::vector<uint32_t>>();
        auto morphologies = _file.getDataSet("network/neurons/morphology").read<std::vector<std::string>>();

        std::unordered_map<std::string, std::shared_ptr<Morphology>> loaded;

        for (size_t i = 0; i < ids.size(); ++i) {
            auto id = ids[i];
            auto& name = morphologies[i];
            if (loaded.contains(name)) {
                assignMorphology(dataset, id, loaded[name]);
                continue;
            }

            std::string modified = name;
            modified.replace(0, SNUDDA_PREFIX.length(), _dataPath.string());
            std::filesystem::path path(modified);

            SWCLoader loader(path);

            auto swc = loader.loadMorphology(dataset);
            if (!swc.isOk()) {
                return swc.getError();
            }
            loaded[name] = swc.getResult();
            assignMorphology(dataset, id, swc.getResult());
        }

        return {};
    }

    std::optional<std::string> SnuddaLoader::loadSynapses(Dataset& dataset,
                                                          const SnuddaLoaderProperties& properties) const
    {
        using Syn = std::array<int32_t, 13>;
        auto voxelSize = static_cast<float>(_file.getDataSet("meta/voxel_size").read<double>());
        auto synapses = _file.getDataSet("network/synapses").read<std::vector<Syn>>();
        auto origo = _file.getDataSet("meta/simulation_origo").read<std::array<double, 3>>();

        auto origin = rush::Vec3f(origo[0], origo[1], origo[2]) * METER_MICROMETER_RATIO;

        std::unordered_multimap<UID, std::pair<UID, rush::Vec3f>> synapsesMap;

        UID uidGenerator = 0;

        for (const auto& synapse : synapses) {
            UID sourceId = synapse[0];
            UID destId = synapse[1];
            int32_t destSegId = synapse[9];

            rush::Vec3f position =
                (rush::Vec3f(synapse[2], synapse[3], synapse[4]) * voxelSize + origin) * METER_MICROMETER_RATIO;

            Synapse syn(uidGenerator++, sourceId, destId);
            syn.setProperty(properties.neuritePosition, position);

            if (destSegId >= 0) {
                syn.setProperty(properties.synapsePostSection, static_cast<UID>(destSegId) + 1);
            }

            synapsesMap.insert({
                sourceId, {syn.getUID(), position}
            });
            dataset.getCircuit().addSynapse(std::move(syn));
        }

        for (auto it = synapsesMap.begin(); it != synapsesMap.end();) {
            const UID neuronUID = it->first;
            auto range = synapsesMap.equal_range(neuronUID);
            auto neuronOpt = dataset.getNeuron(neuronUID);

            if (neuronOpt.has_value() && neuronOpt.value()->getMorphology().has_value()) {
                auto neuron = neuronOpt.value();
                std::vector<rush::Vec3f> points;
                auto transformOpt = neuron->getProperty<NeuronTransform>(properties.neuronTransform);

                for (auto gIt = range.first; gIt != range.second; ++gIt) {
                    points.push_back(gIt->second.second);
                }

                auto results = closestNeuriteToPosition(dataset, *neuron->getMorphology().value(), points,
                                                        transformOpt ? &transformOpt.value() : nullptr);

                size_t idx = 0;
                for (auto gIt = range.first; gIt != range.second; ++gIt, ++idx) {
                    if (!results[idx].valid) {
                        continue;
                    }

                    auto synapseOpt = dataset.getCircuit().getSynapse(gIt->second.first);
                    if (!synapseOpt) {
                        continue;
                    }

                    synapseOpt.value()->setProperty(properties.synapsePreSection, results[idx].uid);
                }
            }

            it = range.second;
        }

        return {};
    }

    void SnuddaLoader::assignMorphology(Dataset& dataset, UID uid, std::shared_ptr<Morphology> morphology)
    {
        if (auto neuron = dataset.getNeuron(uid); neuron.has_value()) {
            neuron.value()->setMorphology(std::move(morphology));
        }
    }

    SnuddaLoader::SnuddaLoader(const std::filesystem::path& path) :
        _file(path.string(), HighFive::File::ReadOnly),
        _dataPath(path.parent_path() / "data"),
        _loadMorphology(true),
        _loadSynapses(true)
    {
    }

    bool SnuddaLoader::shouldLoadMorphology() const
    {
        return _loadMorphology;
    }

    void SnuddaLoader::setLoadMorphology(bool loadMorphology)
    {
        _loadMorphology = loadMorphology;
    }

    bool SnuddaLoader::shouldLoadSynapses() const
    {
        return _loadSynapses;
    }

    void SnuddaLoader::setLoadSynapses(bool loadSynapses)
    {
        _loadSynapses = loadSynapses;
    }

    void SnuddaLoader::load(Dataset& dataset) const
    {
        SnuddaLoaderProperties properties = initProperties(dataset.getProperties());

        loadNeurons(dataset, properties);

        if (_loadMorphology) {
            auto error = loadMorphologies(dataset);
            if (error.has_value()) {
                std::cerr << error.value() << std::endl;
                return;
            }
        }

        if (_loadSynapses) {
            auto error = loadSynapses(dataset, properties);
            if (error.has_value()) {
                std::cerr << error.value() << std::endl;
                return;
            }
        }
    }

    LoaderFactory SnuddaLoader::createFactory()
    {
        return LoaderFactory(
            SNUDDA_LOADER_ID, SNUDDA_LOADER_NAME, false,
            [](const std::string& name) {
                std::string extension = std::filesystem::path(name).extension().string();
                return extension == ".h5" || extension == ".hdf5";
            },
            [](LoaderFactory::FileProvider, const std::filesystem::path& path) {
                return LoaderFactory::FactoryResult(std::make_unique<SnuddaLoader>(path));
            });
    }
} // namespace mindset
