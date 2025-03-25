//
// Created by gaeqs on 14/03/25.
//

#include <mnemea/DefaultProperties.h>
#include <mnemea/loader/SnuddaLoader.h>
#include <mnemea/loader/SWCLoader.h>
#include <mnemea/util/NeuronTransform.h>
#include <rush/matrix/mat.h>
#include <rush/vector/vec.h>

struct Syn
{
    int32_t sourceCell;
    int32_t destCell;
    int32_t voxelX;
    int32_t voxelY;
    int32_t voxelZ;
    int32_t hyperVoxelId;
    int32_t channelModelId;
    int32_t sourceAxonSomaDist;
    int32_t destSegId;
    int32_t destSegX;
    int32_t conductance;
    int32_t parameterId;
};

namespace mnemea
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

        result.neuronTransform = properties.defineProperty(PROPERTY_TRANSFORM);
        return result;
    }

    void SnuddaLoader::loadNeurons(Dataset& dataset, const SnuddaLoaderProperties& properties) const
    {
        auto ids = _file.getDataSet("network/neurons/neuron_id").read<std::vector<uint32_t>>();
        auto position = _file.getDataSet("network/neurons/position").read<std::vector<std::array<double, 3>>>();
        auto rotation = _file.getDataSet("network/neurons/rotation").read<std::vector<std::array<double, 9>>>();

        for (size_t i = 0; i < ids.size(); ++i) {
            rush::Vec3f pos(position[i][0], position[i][1], position[i][2]);
            rush::Mat3f rot([array = rotation[i]](size_t c, size_t r) { return static_cast<float>(array[c + r * 3]); });

            rush::Mat4f model(rot, 1.0f);
            model[3] = rush::Vec4f(pos, 1.0f);

            Neuron neuron(ids[i]);
            neuron.setPropertyAsAny(properties.neuronTransform, NeuronTransform(model));
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

    void SnuddaLoader::assignMorphology(Dataset& dataset, UID uid, std::shared_ptr<Morphology> morphology)
    {
        if (auto neuron = dataset.getNeuron(uid); neuron.has_value()) {
            neuron.value()->setMorphology(std::move(morphology));
        }
    }

    SnuddaLoader::SnuddaLoader(const std::filesystem::path& path) :
        _file(path, HighFive::File::ReadOnly),
        _dataPath(path.parent_path() / "data"),
        _loadMorphology(true)
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

    void SnuddaLoader::load(Dataset& dataset) const
    {
        SnuddaLoaderProperties properties = initProperties(dataset.getProperties());

        loadNeurons(dataset, properties);

        if (_loadMorphology) {
            auto error = loadMorphologies(dataset);
            if (error.has_value()) {
                std::cerr << error.value() << std::endl;
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
} // namespace mnemea
