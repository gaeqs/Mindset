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

#ifdef MINDSET_BRION

    #include <mindset/util/MorphologyUtils.h>
    #include <mindset/util/NeuronTransform.h>
    #include <mindset/loader/BlueConfigLoader.h>
    #include <mindset/DefaultProperties.h>

    #include <brain/brain.h>
    #include <rush/rush.h>

namespace
{

    struct RequiredSynapsePositionData
    {
        std::optional<mindset::NeuronTransform> transform;
        const mindset::Neurite* neurite;
        const mindset::Neurite* child;
    };

    bool hasPositions(const brain::Synapses& synapses)
    {
        try {
            return synapses.preSurfaceXPositions() != nullptr;
        } catch (const std::exception& e) {
            return false;
        }
    }

    std::optional<RequiredSynapsePositionData> getNeuriteAndChild(const mindset::Dataset& dataset,
                                                                  const mindset::BlueConfigLoaderProperties& properties,
                                                                  mindset::UID neuronId, mindset::UID sectionId,
                                                                  size_t index)
    {
        auto neuron = dataset.getNeuron(neuronId);
        if (!neuron) {
            return {};
        }

        auto morphology = getMorphology(dataset, neuronId);

        if (!morphology) {
            return {};
        }

        auto section = getMorphologyTreeSection(morphology.value(), sectionId);
        if (!section) {
            return {};
        }

        auto transform = neuron.value()->getProperty<mindset::NeuronTransform>(properties.neuronTransform);

        auto neurites = section.value()->getNeurites();
        if (neurites.size() - 1 < index) {
            if (neurites.size() - 1 == index) {
                // If last
                mindset::UID neuriteId = neurites[index];
                auto neurite = morphology.value()->getNeurite(neuriteId);
                return {
                    {transform, *neurite, *neurite}
                };
            }

            return {};
        }

        mindset::UID neuriteId = neurites[index];
        mindset::UID childId = neurites[index + 1];

        auto neurite = morphology.value()->getNeurite(neuriteId);
        auto child = morphology.value()->getNeurite(childId);

        if (!neurite || !child) {
            return {};
        }

        return {
            {transform, *neurite, *child}
        };
    }

    std::optional<rush::Vec3f> calculatePosition(const mindset::BlueConfigLoaderProperties& properties,
                                                 const mindset::Neurite* neurite, const mindset::Neurite* child,
                                                 float distance)
    {
        auto start = neurite->getProperty<rush::Vec3f>(properties.position);
        auto end = child->getProperty<rush::Vec3f>(properties.position);

        if (!start || !end) {
            return {};
        }

        auto direction = (*end - *start).normalized();
        return *start + direction * distance;
    }
} // namespace

namespace mindset
{
    BlueConfigLoaderProperties BlueConfigLoader::initProperties(Dataset& dataset, bool shouldLoadMorphologies,
                                                                bool shouldLoadSynapses, bool shouldLoadHierarchy) const
    {
        auto lock = dataset.writeLock();
        auto& properties = dataset.getProperties();
        BlueConfigLoaderProperties result{};

        // Used for neurites and synapses.

        if (shouldLoadMorphologies || shouldLoadSynapses) {
            result.position = properties.defineProperty(PROPERTY_POSITION);
        }

        if (shouldLoadMorphologies) {
            result.neuriteRadius = properties.defineProperty(PROPERTY_RADIUS);
            result.neuriteParent = properties.defineProperty(PROPERTY_PARENT);
            result.neuriteType = properties.defineProperty(PROPERTY_NEURITE_TYPE);
        }

        result.neuronTransform = properties.defineProperty(PROPERTY_TRANSFORM);
        result.neuronLayer = properties.defineProperty(PROPERTY_LAYER);

        if (shouldLoadHierarchy) {
            result.neuronColumn = properties.defineProperty(PROPERTY_COLUMN);
            result.neuronMiniColumn = properties.defineProperty(PROPERTY_MINI_COLUMN);
        }

        if (shouldLoadSynapses) {
            result.synapsePreNeurite = properties.defineProperty(PROPERTY_SYNAPSE_PRE_NEURITE);
            result.synapsePostNeurite = properties.defineProperty(PROPERTY_SYNAPSE_POST_NEURITE);
            result.synapsePrePosition = properties.defineProperty(PROPERTY_SYNAPSE_PRE_POSITION);
            result.synapsePostPosition = properties.defineProperty(PROPERTY_SYNAPSE_POST_POSITION);
            result.synapseDelay = properties.defineProperty(PROPERTY_SYNAPSE_DELAY);
            result.synapseConductance = properties.defineProperty(PROPERTY_SYNAPSE_CONDUCTANCE);
            result.synapseUtilization = properties.defineProperty(PROPERTY_SYNAPSE_UTILIZATION);
            result.synapseDepression = properties.defineProperty(PROPERTY_SYNAPSE_DEPRESSION);
            result.synapseFacilitation = properties.defineProperty(PROPERTY_SYNAPSE_FACILITATION);
            result.synapseDecay = properties.defineProperty(PROPERTY_SYNAPSE_DECAY);
            result.synapseEfficacy = properties.defineProperty(PROPERTY_SYNAPSE_EFFICACY);
        }

        return result;
    }

    void BlueConfigLoader::loadNeurons(Dataset& dataset, const BlueConfigLoaderProperties& properties,
                                       const brion::GIDSet& ids, const brain::Circuit& circuit,
                                       const std::map<std::string, std::shared_ptr<Morphology>>& morphologies)
    {
        auto transforms = circuit.getTransforms(ids);
        auto layers = circuit.getLayers(ids);
        auto uris = circuit.getMorphologyURIs(ids);

        size_t index = 0;
        auto lock = dataset.writeLock();
        for (UID id : ids) {
            UID layer = std::stoi(layers[index]);
            if (auto presentNeuron = dataset.getNeuron(id)) {
                auto neuronLock = presentNeuron.value()->writeLock();
                presentNeuron.value()->setProperty(properties.neuronTransform, NeuronTransform(transforms[index]));
                presentNeuron.value()->setProperty(properties.neuronLayer, layer);

                if (auto morphology = morphologies.find(uris[index].getPath()); morphology != morphologies.end()) {
                    presentNeuron.value()->setMorphology(morphology->second);
                }
            } else {
                auto neuron = Neuron(id);
                neuron.setProperty(properties.neuronTransform, NeuronTransform(transforms[index]));
                neuron.setProperty(properties.neuronLayer, layer);

                if (auto morphology = morphologies.find(uris[index].getPath()); morphology != morphologies.end()) {
                    neuron.setMorphology(morphology->second);
                }
                dataset.addNeuron(std::move(neuron));
            }
            ++index;
        }
    }

    std::map<std::string, std::shared_ptr<Morphology>> BlueConfigLoader::loadMorphologies(
        const BlueConfigLoaderProperties& properties, const brion::GIDSet& ids, const brain::Circuit& circuit)
    {
        auto uris = circuit.getMorphologyURIs(ids);
        auto transforms = circuit.getTransforms(ids);
        auto layers = circuit.getLayers(ids);

        std::map<std::string, brion::URI> files;

        for (const auto& uri : uris) {
            files.insert({uri.getPath(), uri});
        }

        std::map<std::string, std::shared_ptr<Morphology>> morphologies;
        for (auto& [file, uri] : files) {
            morphologies[file] = loadMorphology(properties, brion::Morphology(uri));
        }

        return morphologies;
    }

    void BlueConfigLoader::loadSynapses(Dataset& dataset, const BlueConfigLoaderProperties& properties,
                                        const brion::GIDSet& ids, const brain::Circuit& circuit)
    {
        auto brainSynapses = circuit.getAfferentSynapses(ids, brain::SynapsePrefetch::attributes);
        auto future = brainSynapses.read(brainSynapses.getRemaining());
        auto synapses = future.get();

        auto& outCircuit = dataset.getCircuit();

        UID uidGenerator = 0;

        bool usePositions = hasPositions(synapses);

        for (auto synapse : synapses) {
            Synapse result(uidGenerator++, synapse.getPresynapticGID(), synapse.getPostsynapticGID());

            auto preNeurites = getNeuriteAndChild(dataset, properties, synapse.getPresynapticGID(),
                                                  synapse.getPresynapticSectionID(), synapse.getPresynapticSegmentID());

            auto postNeurites =
                getNeuriteAndChild(dataset, properties, synapse.getPostsynapticGID(),
                                   synapse.getPostsynapticSectionID(), synapse.getPostsynapticSegmentID());

            if (preNeurites) {
                // Brion uses neurite to children. Mindset uses neuron to parent.
                // Because of that, we have to set that the neurite that has the synapse is the child.
                result.setProperty(properties.synapsePostNeurite, preNeurites->child->getUID());
            }

            if (postNeurites) {
                // Brion uses neurite to children. Mindset uses neuron to parent.
                // Because of that, we have to set that the neurite that has the synapse is the child.
                result.setProperty(properties.synapsePostNeurite, postNeurites->child->getUID());
            }

            if (usePositions) {
                result.setProperty(properties.position, synapse.getPresynapticCenterPosition());
                result.setProperty(properties.synapsePrePosition, synapse.getPresynapticSurfacePosition());
                result.setProperty(properties.synapsePrePosition, synapse.getPostsynapticSurfacePosition());
            } else {
                // Calculate positions!
                if (preNeurites) {
                    auto& [transform, neurite, child] = preNeurites.value();

                    auto position = calculatePosition(properties, neurite, child, synapse.getPresynapticDistance());
                    if (position) {
                        if (transform) {
                            *position = transform->positionToGlobalCoordinates(*position);
                        }
                        result.setProperty(properties.position, *position);
                        result.setProperty(properties.synapsePrePosition, *position);
                    }
                }

                if (postNeurites) {
                    auto& [transform, neurite, child] = postNeurites.value();

                    auto position = calculatePosition(properties, neurite, child, synapse.getPostsynapticDistance());
                    if (position) {
                        if (transform) {
                            *position = transform->positionToGlobalCoordinates(*position);
                        }
                        result.setProperty(properties.position, *position);
                        result.setProperty(properties.synapsePostPosition, *position);
                    }
                }
            }

            result.setProperty(properties.synapseDelay, synapse.getDelay());
            result.setProperty(properties.synapseConductance, synapse.getConductance());
            result.setProperty(properties.synapseUtilization, synapse.getUtilization());
            result.setProperty(properties.synapseDepression, synapse.getDepression());
            result.setProperty(properties.synapseFacilitation, synapse.getFacilitation());
            result.setProperty(properties.synapseDecay, synapse.getDecay());
            result.setProperty(properties.synapseEfficacy, synapse.getEfficacy());

            auto lock = outCircuit.writeLock();
            outCircuit.addSynapse(std::move(result));
        }
    }

    void BlueConfigLoader::loadHierarchy(Dataset& dataset, const BlueConfigLoaderProperties& properties,
                                         const brion::GIDSet& ids, const brion::Circuit& circuit)
    {
        auto data = circuit.get(ids, brion::NEURON_COLUMN_GID | brion::NEURON_MINICOLUMN_GID);

        Node* root = dataset.getHierarchy().value_or(nullptr);
        if (root == nullptr) {
            root = dataset.createHierarchy(0, "mindset:root");
        }

        size_t index = 0;
        for (UID id : ids) {
            if (auto neuron = dataset.getNeuron(id); neuron.has_value()) {
                auto sub = data[index];

                UID column = boost::lexical_cast<UID>(sub[0]);
                UID miniColumn = boost::lexical_cast<UID>(sub[1]);

                if (auto columnResult = root->getOrCreateNode(column, "mindset:column"); columnResult.isOk()) {
                    if (auto miniColumnResult =
                            columnResult.getResult()->getOrCreateNode(miniColumn, "mindset:mini_column");
                        miniColumnResult.isOk()) {
                        miniColumnResult.getResult()->addNeuron(id);
                    }
                }

                auto lock = neuron.value()->writeLock();
                neuron.value()->setProperty(properties.neuronColumn, column);
                neuron.value()->setProperty(properties.neuronMiniColumn, miniColumn);
            }
            ++index;
        }
    }

    std::shared_ptr<Morphology> BlueConfigLoader::loadMorphology(const BlueConfigLoaderProperties& properties,
                                                                 const brion::Morphology& morphology)
    {
        auto result = std::make_shared<Morphology>();
        MorphologyTree tree;

        auto& points = morphology.getPoints();
        auto& sections = morphology.getSections();
        auto& types = morphology.getSectionTypes();

        std::vector<std::optional<UID>> lastNeurites;
        lastNeurites.resize(sections.size(), 0);

        size_t idGenerator = 0;
        for (size_t i = 0; i < sections.size(); ++i) {
            MorphologyTreeSection section(i);
            int fatherSectionId = sections[i].y;
            auto sectionType = static_cast<NeuriteType>(types[i]);

            if (fatherSectionId >= 0) {
                if (auto parent = tree.getSection(fatherSectionId); parent.has_value()) {
                    section.setParentSection(fatherSectionId);
                    parent.value()->addChildSection(i);
                }
            }

            size_t startNode = sections[i].x;
            size_t endNode = i + 1 >= sections.size() ? points.size() : sections[i + 1].x;

            if (sectionType == NeuriteType::SOMA) {
                // Handle Soma creation
                Soma soma(idGenerator++);
                lastNeurites[i] = soma.getUID();
                for (size_t p = startNode; p < endNode; ++p) {
                    auto point = points[p];
                    soma.addNode({rush::Vec3f(point.x, point.y, point.z), point.w / 2.0f});
                }
                result->setSoma(std::move(soma));

                section.addNeurite(soma.getUID());
            } else {
                auto previous = fatherSectionId >= 0 ? lastNeurites[fatherSectionId] : std::optional<UID>();
                for (size_t p = startNode; p < endNode; ++p) {
                    auto point = points[p];
                    Neurite neurite(idGenerator++);
                    neurite.setProperty(properties.position, rush::Vec3f(point.x, point.y, point.z));
                    neurite.setProperty(properties.neuriteRadius, point.w / 2.0f); // Brion returns the diameter!
                    if (previous.has_value()) {
                        neurite.setProperty(properties.neuriteParent, previous.value());
                    }
                    neurite.setProperty(properties.neuriteType, sectionType);

                    section.addNeurite(neurite.getUID());
                    previous = neurite.getUID();
                    result->addNeurite(std::move(neurite));
                    if (p == startNode) {
                        lastNeurites[i] = previous;
                    }
                }
            }

            tree.addSection(std::move(section));
        }

        result->setMorphologyTree(tree);

        return result;
    }

    BlueConfigLoader::BlueConfigLoader(const LoaderCreateInfo& info, std::filesystem::path path) :
        Loader(info),
        _blueConfig(std::move(path))
    {
    }

    void BlueConfigLoader::load(Dataset& dataset) const
    {
        constexpr size_t STAGES = 5;

        bool shouldLoadMorphologies = getEnvironmentEntryOr(BLUE_CONFIG_LOADER_ENTRY_LOAD_MORPHOLOGY, false);
        bool shouldLoadSynapses = getEnvironmentEntryOr(BLUE_CONFIG_LOADER_ENTRY_LOAD_SYNAPSES, false);
        bool shouldLoadHierarchy = getEnvironmentEntryOr(BLUE_CONFIG_LOADER_ENTRY_LOAD_HIERARCHY, false);

        auto targets = getEnvironmentEntry<std::vector<std::string>>(BLUE_CONFIG_LOADER_ENTRY_TARGETS);
        if (!targets.has_value() || targets.value()->empty()) {
            invoke({LoaderStatusType::LOADING_ERROR, "No targets found", STAGES, 0});
        }

        invoke({LoaderStatusType::LOADING, "Loading targets", STAGES, 0});

        brion::GIDSet ids;
        for (auto& target : *targets.value()) {
            brion::GIDSet targetSet = _blueConfig.parseTarget(target);
            ids.insert(targetSet.begin(), targetSet.end());
        }
        if (ids.empty()) {
            invoke({LoaderStatusType::LOADING_ERROR, "No neurons found", STAGES, 0});
            return;
        }

        invoke({LoaderStatusType::LOADING, "Defining properties", STAGES, 1});

        auto properties = initProperties(dataset, shouldLoadMorphologies, shouldLoadSynapses, shouldLoadHierarchy);

        invoke({LoaderStatusType::LOADING, "Loading global neuron data", STAGES, 2});

        {
            auto circuit = brain::Circuit(_blueConfig);

            std::map<std::string, std::shared_ptr<Morphology>> morphologies;

            if (shouldLoadMorphologies) {
                invoke({LoaderStatusType::LOADING, "Loading morphologies", STAGES, 3});
                morphologies = loadMorphologies(properties, ids, circuit);
            }

            loadNeurons(dataset, properties, ids, circuit, morphologies);

            if (shouldLoadSynapses) {
                loadSynapses(dataset, properties, ids, circuit);
            }
        }

        if (shouldLoadHierarchy) {
            invoke({LoaderStatusType::LOADING, "Loading hierarchy", STAGES, 4});
            auto circuit = brion::Circuit(_blueConfig.getCircuitSource());
            loadHierarchy(dataset, properties, ids, circuit);
        }

        invoke({LoaderStatusType::DONE, "Done", STAGES, 5});
    }

    LoaderFactory BlueConfigLoader::createFactory()
    {
        std::vector<LoaderEnvironmentEntry> entries = {
            {        .name = BLUE_CONFIG_LOADER_ENTRY_TARGETS,
             .displayName = "Targets",
             .type = typeid(std::vector<std::string>),
             .defaultValue = std::vector<std::string>{},
             .hint = {}},
            {.name = BLUE_CONFIG_LOADER_ENTRY_LOAD_MORPHOLOGY,
             .displayName = "Load morphology",
             .type = typeid(bool),
             .defaultValue = true,
             .hint = {}},
            { .name = BLUE_CONFIG_LOADER_ENTRY_LOAD_HIERARCHY,
             .displayName = "Load hierarchy",
             .type = typeid(bool),
             .defaultValue = true,
             .hint = {}},
            {  .name = BLUE_CONFIG_LOADER_ENTRY_LOAD_SYNAPSES,
             .displayName = "Load synapses",
             .type = typeid(bool),
             .defaultValue = true,
             .hint = {}},
        };

        return LoaderFactory(
            BLUE_CONFIG_LOADER_ID, BLUE_CONFIG_LOADER_NAME, true, entries,
            [](const std::string& name) { return name == "BlueConfig"; },
            [](const LoaderCreateInfo& info, const std::filesystem::path& path) {
                return FactoryResult(std::make_unique<BlueConfigLoader>(info, path));
            });
    }
} // namespace mindset

#endif
