// Copyright (c) 2025. VG-Lab/URJC.
//
// Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
//
// This file is part of Mindset <https://gitlab.gmrv.es/g.rial/mindset>
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 3.0 as published
// by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include "mindset/EventSequence.h"
#include "mindset/TimeGrid.h"

#include <mindset/DefaultProperties.h>
#include <mindset/loader/SnuddaLoader.h>
#include <mindset/loader/SWCLoader.h>
#include <mindset/util/NeuronTransform.h>
#include <mindset/util/MorphologyUtils.h>
#include <rush/matrix/mat.h>
#include <rush/vector/vec.h>

namespace
{
    constexpr float METER_MICROMETER_RATIO = 1'000'000.0f;

    template<typename Collection>
    std::optional<std::string> fetchValidGroup(const HighFive::File& file, const Collection& groups)
    {
        for (const auto& group : groups) {
            if (file.exist(group)) {
                return group;
            }
        }

        return {};
    }

    std::optional<std::vector<uint64_t>> tryFetchUIDs(const HighFive::File& file, std::optional<std::string> idGroup)
    {
        std::vector<uint64_t> values;
        if (idGroup.has_value()) {
            // Fetch using the dataset group.
            file.getDataSet(idGroup.value()).read(values);
            return values;
        }

        // Check if it is an input or an output file
        std::string groupName;
        if (file.exist("neurons")) {
            groupName = "neurons";
        } else if (file.exist("input")) {
            groupName = "input";
        } else {
            return {};
        }

        auto group = file.getGroup(groupName);
        auto names = group.listObjectNames(HighFive::IndexType::NAME);
        values.reserve(names.size());

        for (auto& name : names) {
            if (group.getObjectType(name) == HighFive::ObjectType::Group) {
                uint64_t value;
                auto result = std::from_chars(name.data(), name.data() + name.size(), value);
                if (result.ec == std::errc()) {
                    values.push_back(value);
                }
            }
        }

        return values;
    }

    std::vector<std::string> readMorphologies(const HighFive::File& file, const std::string& group)
    {
        try {
            return file.getDataSet(group).read<std::vector<std::string>>();
        } catch (HighFive::DataSetException&) {
            // Read it using fixed strings.
            auto raw = file.getDataSet(group).read<HighFive::FixedLenStringArray<1024>>();
            std::vector<std::string> result;
            result.reserve(raw.size());
            for (auto& name : raw) {
                result.push_back(std::string(name.begin(), name.end()));
            }
            return result;
        }
    }
} // namespace

namespace mindset
{

    SnuddaLoaderProperties SnuddaLoader::initProperties(Dataset& dataset, const std::string& snuddaPath,
                                                        std::vector<uint64_t> ids) const
    {
        SnuddaLoaderProperties result{};
        result.snuddaPath = snuddaPath;
        result.ids = std::move(ids);
        result.loadMorphologies = getEnvironmentEntryOr(SNUDDA_LOADER_ENTRY_LOAD_MORPHOLOGY, false);
        result.loadSynapses = getEnvironmentEntryOr(SNUDDA_LOADER_ENTRY_LOAD_SYNAPSES, false);
        result.loadActivity = getEnvironmentEntryOr(SNUDDA_LOADER_ENTRY_LOAD_ACTIVITY, false);

        result.positionGroup = fetchValidGroup(_file, SNUDDA_LOADER_VALID_POSITION_GROUPS);
        result.rotationGroup = fetchValidGroup(_file, SNUDDA_LOADER_VALID_ROTATION_GROUPS);
        result.morphologyGroup = fetchValidGroup(_file, SNUDDA_LOADER_VALID_MORPHOLOGY_GROUPS);

        result.voxelSizeGroup = fetchValidGroup(_file, SNUDDA_LOADER_VALID_VOXEL_SIZE_GROUPS);
        result.synapsesGroup = fetchValidGroup(_file, SNUDDA_LOADER_VALID_SYNAPSES_GROUPS);
        result.simulationOrigoGroup = fetchValidGroup(_file, SNUDDA_LOADER_VALID_SIMULATION_ORIGO_GROUPS);

        auto lock = dataset.writeLock();
        auto& properties = dataset.getProperties();

        if (result.loadSynapses || result.loadMorphologies) {
            result.position = properties.defineProperty(PROPERTY_POSITION);
        }

        if (result.loadMorphologies) {
            result.neuriteRadius = properties.defineProperty(PROPERTY_RADIUS);
            result.neuriteParent = properties.defineProperty(PROPERTY_PARENT);
            result.neuriteType = properties.defineProperty(PROPERTY_NEURITE_TYPE);
        }

        if (result.loadSynapses) {
            result.synapsePreNeurite = properties.defineProperty(PROPERTY_SYNAPSE_PRE_NEURITE);
            result.synapsePostNeurite = properties.defineProperty(PROPERTY_SYNAPSE_POST_NEURITE);
            result.synapsePrePosition = properties.defineProperty(PROPERTY_SYNAPSE_PRE_POSITION);
            result.synapsePostPosition = properties.defineProperty(PROPERTY_SYNAPSE_POST_POSITION);
        }

        if (result.loadActivity) {
            result.activitySpikes = properties.defineProperty(PROPERTY_ACTIVITY_SPIKES);
            result.activityVoltage = properties.defineProperty(PROPERTY_ACTIVITY_VOLTAGE);
        }

        result.neuronTransform = properties.defineProperty(PROPERTY_TRANSFORM);

        return result;
    }

    void SnuddaLoader::loadNeurons(Dataset& dataset,
                                   const std::unordered_map<std::string, std::shared_ptr<Morphology>>& morphologies,
                                   const SnuddaLoaderProperties& properties) const
    {
        auto& ids = properties.ids;
        std::vector<std::array<double, 3>> positions;
        std::vector<std::array<double, 9>> rotations;
        std::vector<std::string> morphologiesNames;

        if (properties.positionGroup.has_value()) {
            _file.getDataSet(properties.positionGroup.value()).read(positions);
        }
        if (properties.rotationGroup.has_value()) {
            _file.getDataSet(properties.rotationGroup.value()).read(rotations);
        }
        if (properties.morphologyGroup.has_value()) {
            morphologiesNames = readMorphologies(_file, properties.morphologyGroup.value());
        }

        for (size_t i = 0; i < ids.size(); ++i) {
            bool hasPosition = i < positions.size();
            bool hasRotation = i < rotations.size();
            bool hasMorphology = i < morphologiesNames.size();

            std::optional<NeuronTransform> transform = {};
            std::shared_ptr<Morphology> morphology = nullptr;
            if (hasPosition || hasRotation) {
                rush::Mat4f model(1.0f);
                if (hasRotation) {
                    rush::Mat3f rot(
                        [array = rotations[i]](size_t c, size_t r) { return static_cast<float>(array[c + r * 3]); });
                    model = rush::Mat4f(rot, 1.0f);
                }

                if (hasPosition) {
                    rush::Vec3f pos(positions[i][0], positions[i][1], positions[i][2]); // Position in meters
                    model[3] = rush::Vec4f(pos * METER_MICROMETER_RATIO, 1.0f);
                }
                transform = NeuronTransform(model);
            }

            if (hasMorphology) {
                if (auto it = morphologies.find(std::string(morphologiesNames[i])); it != morphologies.end()) {
                    morphology = it->second;
                }
            }

            auto lock = dataset.writeLock();
            auto presentNeuron = dataset.getNeuron(ids[i]);
            if (presentNeuron.has_value()) {
                auto neuronLock = presentNeuron.value()->writeLock();
                if (morphology != nullptr) {
                    presentNeuron.value()->setMorphology(morphology);
                }
                if (transform.has_value()) {
                    presentNeuron.value()->setProperty(properties.neuronTransform, transform.value());
                }
            } else {
                Neuron neuron(ids[i], morphology);
                if (transform.has_value()) {
                    neuron.setProperty(properties.neuronTransform, transform.value());
                }
                dataset.addNeuron(std::move(neuron));
            }
        }
    }

    Result<std::unordered_map<std::string, std::shared_ptr<Morphology>>, std::string> SnuddaLoader::loadMorphologies(
        const SnuddaLoaderProperties& properties, Dataset& dataset) const
    {
        static constexpr std::string SNUDDA_PREFIX = "$SNUDDA_DATA";

        std::unordered_map<std::string, std::shared_ptr<Morphology>> loaded;
        if (!properties.morphologyGroup.has_value()) {
            // This is not an error; it just happens that the dataset doesn't have morphologies!
            return std::move(loaded);
        }

        auto morphologies = readMorphologies(_file, properties.morphologyGroup.value());

        for (size_t i = 0; i < morphologies.size(); ++i) {
            auto name = morphologies[i];
            if (loaded.contains(name)) {
                continue;
            }

            std::string modified = name;
            modified.replace(0, SNUDDA_PREFIX.length(), properties.snuddaPath);
            std::filesystem::path path(modified);

            SWCLoader loader(LoaderCreateInfo(), path);

            auto swc = loader.loadMorphology(dataset);
            if (!swc.isOk()) {
                return swc.getError();
            }
            loaded[name] = swc.getResult();
        }

        return std::move(loaded);
    }

    void SnuddaLoader::loadSynapses(Dataset& dataset, const SnuddaLoaderProperties& properties) const
    {
        if (!properties.voxelSizeGroup.has_value() || !properties.synapsesGroup.has_value() ||
            !properties.simulationOrigoGroup.has_value()) {
            return;
        }

        using Syn = std::array<int32_t, 13>;
        auto voxelSize = static_cast<float>(_file.getDataSet(properties.voxelSizeGroup.value()).read<double>());
        auto synapses = _file.getDataSet(properties.synapsesGroup.value()).read<std::vector<Syn>>();
        auto origo = _file.getDataSet(properties.simulationOrigoGroup.value()).read<std::array<double, 3>>();

        auto origin = rush::Vec3f(origo[0], origo[1], origo[2]);

        std::unordered_map<UID, Synapse> _synapses;
        std::unordered_multimap<UID, std::pair<UID, rush::Vec3f>> synapsesMap;

        UID uidGenerator = 0;

        for (const auto& synapse : synapses) {
            UID sourceId = synapse[0];
            UID destId = synapse[1];
            int32_t destSegId = synapse[9];

            rush::Vec3f position =
                (rush::Vec3f(synapse[2], synapse[3], synapse[4]) * voxelSize + origin) * METER_MICROMETER_RATIO;

            Synapse syn(uidGenerator++, sourceId, destId);
            syn.setProperty(properties.position, position);
            syn.setProperty(properties.synapsePostPosition, position);

            if (destSegId >= 0) {
                syn.setProperty(properties.synapsePostNeurite, static_cast<UID>(destSegId) + 1);
            }

            synapsesMap.insert({
                sourceId, {syn.getUID(), position}
            });
            _synapses.insert({syn.getUID(), std::move(syn)});
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

                    auto synapseOpt = _synapses.find(gIt->second.first);
                    if (synapseOpt == _synapses.end()) {
                        continue;
                    }

                    synapseOpt->second.setProperty(properties.synapsePreNeurite, results[idx].uid);
                    synapseOpt->second.setProperty(properties.synapsePrePosition, results[idx].position);
                }
            }

            it = range.second;
        }

        auto& circuit = dataset.getCircuit();
        auto lock = circuit.writeLock();
        for (auto& synapse : _synapses | std::views::values) {
            circuit.addSynapse(std::move(synapse));
        }
    }

    void SnuddaLoader::loadOutputActivity(Dataset& dataset, const SnuddaLoaderProperties& properties) const
    {
        if (!_file.exist("neurons")) {
            return;
        }
        EventSequence<std::monostate> spikes;
        TimeGrid<double> voltage(std::chrono::nanoseconds(25000));

        std::vector<double> rawSpikes;
        std::vector<double> rawVoltage;

        rawSpikes.reserve(1000);
        rawVoltage.reserve(400001);

        for (UID id : properties.ids) {
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

        auto lock = dataset.writeLock();
        Activity activity(dataset.findSmallestAvailableActivityUID());
        activity.setProperty(properties.activitySpikes, std::move(spikes));
        activity.setProperty(properties.activityVoltage, std::move(voltage));
        dataset.addActivity(std::move(activity));
    }

    void SnuddaLoader::loadInputActivity(Dataset& dataset, const SnuddaLoaderProperties& properties) const
    {
        if (!_file.exist("input")) {
            return;
        }
        EventSequence<std::monostate> spikes;
        TimeGrid<double> voltage(std::chrono::nanoseconds(25000));

        std::vector<double> rawSpikes;

        rawSpikes.reserve(1000);

        for (UID id : properties.ids) {
            std::string spikesDataset = std::format("input/{}/activity/spikes", id);

            std::array tableSpikes = {std::format("input/{}/cortical/spikes", id),
                                      std::format("input/{}/cortical_background/spikes", id),
                                      std::format("input/{}/thalamic_background/spikes", id)};

            if (_file.exist(spikesDataset)) {
                _file.getDataSet(spikesDataset).read(rawSpikes);
                for (double time : rawSpikes) {
                    spikes.addEvent(id, std::chrono::duration<double>(time), std::monostate());
                }
            }

            for (auto& set : tableSpikes) {
                if (_file.exist(set)) {
                    auto ds = _file.getDataSet(set);
                    rawSpikes.resize(ds.getElementCount());
                    ds.read(rawSpikes);
                    for (double time : rawSpikes) {
                        if (time > 0) {
                            spikes.addEvent(id, std::chrono::duration<double>(time), std::monostate());
                        }
                    }
                }
            }
        }

        auto lock = dataset.writeLock();
        Activity activity(dataset.findSmallestAvailableActivityUID());
        activity.setProperty(properties.activitySpikes, std::move(spikes));
        dataset.addActivity(std::move(activity));
    }

    SnuddaLoader::SnuddaLoader(const LoaderCreateInfo& info, const std::filesystem::path& path) :
        Loader(info),
        _file(path.string(), HighFive::File::ReadOnly)
    {
    }

    void SnuddaLoader::load(Dataset& dataset) const
    {
        constexpr size_t STAGES = 6;
        invoke({LoaderStatusType::LOADING, "Fetching properties", STAGES, 0});

        auto path = getEnvironmentEntry<std::string>(SNUDDA_LOADER_ENTRY_SNUDDA_DATA_PATH);
        if (!path.has_value()) {
            invoke({LoaderStatusType::LOADING_ERROR, "Snudda path not set.", STAGES, 0});
            return;
        }

        auto ids = tryFetchUIDs(_file, fetchValidGroup(_file, SNUDDA_LOADER_VALID_ID_GROUPS));
        if (!ids.has_value()) {
            invoke({LoaderStatusType::LOADING_ERROR, "Neuron ids not present.", STAGES, 0});
            return;
        }

        auto properties = initProperties(dataset, *path.value(), std::move(ids.value()));

        std::unordered_map<std::string, std::shared_ptr<Morphology>> morphologies;
        if (properties.loadMorphologies) {
            invoke({LoaderStatusType::LOADING, "Loading morphologies", STAGES, 1});
            auto result = loadMorphologies(properties, dataset);
            if (!result.isOk()) {
                std::cerr << result.getError() << std::endl;
                return;
            }

            morphologies = std::move(result.getResult());
        }

        invoke({LoaderStatusType::LOADING, "Loading neurons", STAGES, 2});
        loadNeurons(dataset, morphologies, properties);

        if (properties.loadSynapses) {
            invoke({LoaderStatusType::LOADING, "Loading synapses", STAGES, 3});
            loadSynapses(dataset, properties);
        }

        if (properties.loadActivity) {
            invoke({LoaderStatusType::LOADING, "Loading input activity", STAGES, 4});
            loadInputActivity(dataset, properties);
            invoke({LoaderStatusType::LOADING, "Loading output activity", STAGES, 5});
            loadOutputActivity(dataset, properties);
        }

        invoke({LoaderStatusType::DONE, "Done", STAGES, STAGES});
    }

    LoaderFactory SnuddaLoader::createFactory()
    {
        auto path = getenv("SNUDDA_DATA");
        std::optional<std::any> pathOptional;
        if (path != nullptr) {
            pathOptional = std::string(path);
        }
        std::vector<LoaderEnvironmentEntry> entries = {
            {.name = SNUDDA_LOADER_ENTRY_SNUDDA_DATA_PATH,
             .displayName = "Snudda data path",
             .type = typeid(std::string),
             .defaultValue = pathOptional,
             .hint = {}},
            { .name = SNUDDA_LOADER_ENTRY_LOAD_MORPHOLOGY,
             .displayName = "Load morphology",
             .type = typeid(bool),
             .defaultValue = true,
             .hint = {}},
            {   .name = SNUDDA_LOADER_ENTRY_LOAD_SYNAPSES,
             .displayName = "Load synapses",
             .type = typeid(bool),
             .defaultValue = true,
             .hint = {}},
            {   .name = SNUDDA_LOADER_ENTRY_LOAD_ACTIVITY,
             .displayName = "Load activity",
             .type = typeid(bool),
             .defaultValue = true,
             .hint = {}},
        };

        return LoaderFactory(
            SNUDDA_LOADER_ID, SNUDDA_LOADER_NAME, false, entries,
            [](const std::string& name) {
                std::string extension = std::filesystem::path(name).extension().string();
                return extension == ".h5" || extension == ".hdf5";
            },
            [](const LoaderCreateInfo& info, const std::filesystem::path& path) {
                return FactoryResult(std::make_unique<SnuddaLoader>(info, path));
            });
    }
} // namespace mindset