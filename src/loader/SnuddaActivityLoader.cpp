//
// Created by gaeqs on 14/03/25.
//

#include "mindset/EventSequence.h"
#include "mindset/TimeGrid.h"

#include <format>
#include <mindset/loader/SnuddaActivityLoader.h>
#include <mindset/DefaultProperties.h>
#include <mindset/loader/SWCLoader.h>
#include <mindset/util/NeuronTransform.h>
#include <rush/matrix/mat.h>
#include <rush/vector/vec.h>

namespace
{
    constexpr float METER_MICROMETER_RATIO = 1'000'000.0f;
}

namespace mindset
{

    SnuddaActivityLoaderProperties SnuddaActivityLoader::initProperties(Properties& properties) const
    {
        SnuddaActivityLoaderProperties result{};

        if (_loadMorphology) {
            result.position = properties.defineProperty(PROPERTY_POSITION);
        }

        if (_loadMorphology) {
            result.neuriteRadius = properties.defineProperty(PROPERTY_RADIUS);
            result.neuriteParent = properties.defineProperty(PROPERTY_PARENT);
            result.neuriteType = properties.defineProperty(PROPERTY_NEURITE_TYPE);
        }

        result.neuronTransform = properties.defineProperty(PROPERTY_TRANSFORM);
        result.activitySpikes = properties.defineProperty(PROPERTY_ACTIVITY_SPIKES);
        result.activityVoltage = properties.defineProperty(PROPERTY_ACTIVITY_VOLTAGE);
        return result;
    }

    void SnuddaActivityLoader::loadNeurons(
        Dataset& dataset, const std::unordered_map<std::string, std::shared_ptr<Morphology>>& morphologies,
        const SnuddaActivityLoaderProperties& properties) const
    {
        auto ids = _file.getDataSet("meta_data/id").read<std::vector<uint64_t>>();
        auto position = _file.getDataSet("meta_data/position").read<std::vector<std::array<double, 3>>>();
        auto morphologiesNames = _file.getDataSet("meta_data/morphology").read<HighFive::FixedLenStringArray<1024>>();

        for (size_t i = 0; i < ids.size(); ++i) {
            rush::Vec3f pos(position[i][0], position[i][1], position[i][2]); // Position in meters

            rush::Mat4f model(1.0f);
            model[3] = rush::Vec4f(pos * METER_MICROMETER_RATIO, 1.0f);

            auto morphologyName = morphologiesNames[i];
            auto morphology = morphologies.find(std::string(morphologyName));

            auto presentNeuron = dataset.getNeuron(ids[i]);
            if (presentNeuron.has_value()) {
                if (morphology != morphologies.end()) {
                    presentNeuron.value()->setMorphology(morphology->second);
                }
                presentNeuron.value()->setProperty(properties.neuronTransform, NeuronTransform(model));
            } else {
                Neuron neuron(ids[i]);

                if (morphology != morphologies.end()) {
                    neuron.setMorphology(morphology->second);
                }

                neuron.setProperty(properties.neuronTransform, NeuronTransform(model));
                dataset.addNeuron(std::move(neuron));
            }
        }
    }

    Result<std::unordered_map<std::string, std::shared_ptr<Morphology>>, std::string> SnuddaActivityLoader::
        loadMorphologies(Dataset& dataset) const
    {
        static const std::string SNUDDA_PREFIX = "$SNUDDA_DATA";
        auto morphologies = _file.getDataSet("meta_data/morphology").read<HighFive::FixedLenStringArray<1024>>();

        std::unordered_map<std::string, std::shared_ptr<Morphology>> loaded;

        for (size_t i = 0; i < morphologies.size(); ++i) {
            auto name = std::string(morphologies[i]);
            if (loaded.contains(name)) {
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
        }

        return std::move(loaded);
    }

    void SnuddaActivityLoader::loadActivity(Dataset& dataset, const SnuddaActivityLoaderProperties& properties) const
    {
        Activity activity(0);
        EventSequence<std::monostate> spikes;
        TimeGrid<double> voltage(std::chrono::nanoseconds(25000));

        auto ids = _file.getDataSet("meta_data/id").read<std::vector<uint64_t>>();
        std::vector<double> rawSpikes;
        std::vector<double> rawVoltage;

        rawSpikes.reserve(1000);
        rawVoltage.reserve(400001);

        for (UID id : ids) {
            std::string spikesDataset = std::format("neurons/{}/spikes", id);
            std::string voltageDataset = std::format("neurons/{}/voltage", id);

            if (_file.exist(spikesDataset)) {
                _file.getDataSet(spikesDataset).read(rawSpikes);
                for (double time : rawSpikes) {
                    spikes.addEvent(id, std::chrono::duration<double>(time), std::monostate());
                }
            }

            if (_file.exist(voltageDataset)) {
                _file.getDataSet(voltageDataset).read(rawVoltage);
                voltage.addTimeline(id, rawVoltage);
            }
        }

        activity.setProperty(properties.activitySpikes, std::move(spikes));
        activity.setProperty(properties.activityVoltage, std::move(voltage));
        dataset.addActivity(std::move(activity));
    }

    SnuddaActivityLoader::SnuddaActivityLoader(const std::filesystem::path& path) :
        _file(path.string(), HighFive::File::ReadOnly),
        _dataPath(path.parent_path() / "data"),
        _loadMorphology(true)
    {
    }

    bool SnuddaActivityLoader::shouldLoadMorphology() const
    {
        return _loadMorphology;
    }

    void SnuddaActivityLoader::setLoadMorphology(bool loadMorphology)
    {
        _loadMorphology = loadMorphology;
    }

    void SnuddaActivityLoader::load(Dataset& dataset) const
    {
        SnuddaActivityLoaderProperties properties = initProperties(dataset.getProperties());

        std::unordered_map<std::string, std::shared_ptr<Morphology>> morphologies;

        if (_loadMorphology) {
            auto result = loadMorphologies(dataset);
            if (!result.isOk()) {
                std::cerr << result.getError() << std::endl;
                return;
            }

            morphologies = std::move(result.getResult());
        }

        loadNeurons(dataset, morphologies, properties);
        loadActivity(dataset, properties);
    }

    LoaderFactory SnuddaActivityLoader::createFactory()
    {
        return LoaderFactory(
            SNUDDA_ACTIVITY_LOADER_ID, SNUDDA_ACTIVITY_LOADER_NAME, false,
            [](const std::string& name) {
                std::string extension = std::filesystem::path(name).extension().string();
                return extension == ".h5" || extension == ".hdf5";
            },
            [](LoaderFactory::FileProvider, const std::filesystem::path& path) {
                return LoaderFactory::FactoryResult(std::make_unique<SnuddaActivityLoader>(path));
            });
    }
} // namespace mindset
