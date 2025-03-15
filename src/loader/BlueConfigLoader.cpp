//
// Created by gaeqs on 12/03/25.
//

#ifdef MNEMEA_BRION

#include <mnemea/util/NeuronTransform.h>
#include <mnemea/loader/BlueConfigLoader.h>
#include <mnemea/DefaultProperties.h>

#include <brain/circuit.h>
#include <rush/rush.h>


namespace mnemea {
    BlueConfigLoaderProperties BlueConfigLoader::initProperties(Properties& properties) const {
        BlueConfigLoaderProperties result{};
        if (_loadMorphology) {
            result.neuritePosition = properties.defineProperty(PROPERTY_POSITION);
            result.neuriteRadius = properties.defineProperty(PROPERTY_RADIUS);
            result.neuriteParent = properties.defineProperty(PROPERTY_PARENT);
            result.neuriteType = properties.defineProperty(PROPERTY_NEURITE_TYPE);
        }

        result.neuronTransform = properties.defineProperty(PROPERTY_TRANSFORM);
        result.neuronLayer = properties.defineProperty(PROPERTY_LAYER);

        if (_loadHierarchy) {
            result.neuronColumn = properties.defineProperty(PROPERTY_COLUMN);
            result.neuronMiniColumn = properties.defineProperty(PROPERTY_MINI_COLUMN);
        }
        return result;
    }

    void BlueConfigLoader::loadNeurons(Dataset& dataset,
                                       const BlueConfigLoaderProperties& properties,
                                       const brion::GIDSet& ids,
                                       const brain::Circuit& circuit) {
        auto transforms = circuit.getTransforms(ids);
        auto layers = circuit.getLayers(ids);
        size_t index = 0;
        for (UID id: ids) {
            auto neuron = Neuron(id);
            neuron.setPropertyAsAny(properties.neuronTransform, NeuronTransform(transforms[index]));
            neuron.setPropertyAsAny(properties.neuronLayer, std::stoi(layers[index]));
            dataset.addNeuron(std::move(neuron));
            ++index;
        }
    }

    void BlueConfigLoader::loadMorphologies(Dataset& dataset,
                                            const BlueConfigLoaderProperties& properties,
                                            const brion::GIDSet& ids,
                                            const brain::Circuit& circuit) {
        auto uris = circuit.getMorphologyURIs(ids);
        auto transforms = circuit.getTransforms(ids);
        auto layers = circuit.getLayers(ids);

        std::map<std::string, brion::URI> files;

        for (const auto& uri: uris) {
            files.insert({uri.getPath(), uri});
        }

        std::map<std::string, std::shared_ptr<Morphology>> morphologies;
        for (auto& [file, uri]: files) {
            morphologies[file] = loadMorphology(properties, brion::Morphology(uri));
        }

        size_t index = 0;
        for (UID id: ids) {
            if (auto neuron = dataset.getNeuron(id); neuron.has_value()) {
                if (auto it = morphologies.find(uris[index].getPath()); it != morphologies.end()) {
                    neuron.value()->setMorphology(it->second);
                }
            }
            ++index;
        }
    }

    void BlueConfigLoader::loadHierarchy(Dataset& dataset,
                                         const BlueConfigLoaderProperties& properties,
                                         const brion::GIDSet& ids,
                                         const brion::Circuit& circuit) {
        auto data = circuit.get(ids, brion::NEURON_COLUMN_GID | brion::NEURON_MINICOLUMN_GID);

        Node* root = dataset.getHierarchy().value_or(nullptr);
        if (root == nullptr) {
            root = dataset.createHierarchy(0, "mnemea:root");
        }

        size_t index = 0;
        for (UID id: ids) {
            if (auto neuron = dataset.getNeuron(id); neuron.has_value()) {
                auto sub = data[index];

                UID column = boost::lexical_cast<UID>(sub[0]);
                UID miniColumn = boost::lexical_cast<UID>(sub[1]);

                if (auto columnResult = root->getOrCreateNode(column, "mnemea:column"); columnResult.isOk()) {
                    if (auto miniColumnResult = columnResult.getResult()->getOrCreateNode(miniColumn,
                        "mnemea:mini_column"); miniColumnResult.isOk()) {
                        miniColumnResult.getResult()->addNeuron(id);
                    }
                }

                neuron.value()->setPropertyAsAny(properties.neuronColumn, column);
                neuron.value()->setPropertyAsAny(properties.neuronMiniColumn, miniColumn);
            }
            ++index;
        }
    }

    std::shared_ptr<Morphology> BlueConfigLoader::loadMorphology(const BlueConfigLoaderProperties& properties,
                                                                 const brion::Morphology& morphology) {
        auto result = std::make_shared<Morphology>();

        auto& points = morphology.getPoints();
        auto& sections = morphology.getSections();
        auto& types = morphology.getSectionTypes();

        std::vector<std::optional<UID>> lastNeurites;
        lastNeurites.resize(sections.size(), 0);

        size_t idGenerator = 0;
        for (size_t i = 0; i < sections.size(); ++i) {
            int fatherSectionId = sections[i].y;
            auto sectionType = static_cast<NeuriteType>(types[i]);

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
            } else {
                auto previous = fatherSectionId >= 0 ? lastNeurites[fatherSectionId] : std::optional<UID>();
                for (size_t p = startNode; p < endNode; ++p) {
                    auto point = points[p];
                    Neurite neurite(idGenerator++);
                    neurite.setPropertyAsAny(properties.neuritePosition, rush::Vec3f(point.x, point.y, point.z));
                    neurite.setPropertyAsAny(properties.neuriteRadius, point.w / 2.0f); // Brion returns the diameter!
                    if (previous.has_value()) {
                        neurite.setPropertyAsAny(properties.neuriteParent, previous.value());
                    }
                    neurite.setPropertyAsAny(properties.neuriteType, sectionType);
                    previous = neurite.getUID();
                    result->addNeurite(std::move(neurite));
                    if (p == startNode) {
                        lastNeurites[i] = previous;
                    }
                }
            }
        }

        return result;
    }

    BlueConfigLoader::BlueConfigLoader(std::filesystem::path path):
        _blueConfig(std::move(path)),
        _loadMorphology(true),
        _loadHierarchy(true) {}

    bool BlueConfigLoader::addTarget(std::string target) {
        return _targets.insert(std::move(target)).second;
    }

    std::set<std::string>& BlueConfigLoader::getTargets() {
        return _targets;
    }

    const std::set<std::string>& BlueConfigLoader::getTargets() const {
        return _targets;
    }

    bool BlueConfigLoader::shouldLoadMorphology() const {
        return _loadMorphology;
    }

    void BlueConfigLoader::setLoadMorphology(bool loadMorphology) {
        _loadMorphology = loadMorphology;
    }

    bool BlueConfigLoader::shouldLoadHierarchy() const {
        return _loadHierarchy;
    }

    void BlueConfigLoader::setLoadHierarchy(bool loadHierarchy) {
        _loadHierarchy = loadHierarchy;
    }

    void BlueConfigLoader::load(Dataset& dataset) const {
        constexpr size_t STAGES = 5;
        if (_targets.empty()) {
            invoke({LoaderStatusType::ERROR, "No targets found", STAGES, 0});
        }

        invoke({LoaderStatusType::LOADING, "Loading targets", STAGES, 0});

        brion::GIDSet ids;
        for (auto& target: _targets) {
            brion::GIDSet targetSet = _blueConfig.parseTarget(target);
            ids.insert(targetSet.begin(), targetSet.end());
        }
        if (ids.empty()) {
            invoke({LoaderStatusType::ERROR, "No neurons found", STAGES, 0});
            return;
        }

        invoke({LoaderStatusType::LOADING, "Defining properties", STAGES, 1});

        BlueConfigLoaderProperties properties = initProperties(dataset.getProperties());

        invoke({LoaderStatusType::LOADING, "Loading global neuron data", STAGES, 2});

        /**/ {
            auto circuit = brain::Circuit(_blueConfig);
            loadNeurons(dataset, properties, ids, circuit);

            if (_loadMorphology) {
                invoke({LoaderStatusType::LOADING, "Loading morphologies", STAGES, 3});
                loadMorphologies(dataset, properties, ids, circuit);
            }
        }

        if (_loadHierarchy) {
            invoke({LoaderStatusType::LOADING, "Loading hierarchy", STAGES, 4});
            auto circuit = brion::Circuit(_blueConfig.getCircuitSource());
            loadHierarchy(dataset, properties, ids, circuit);
        }

        invoke({LoaderStatusType::DONE, "Done", STAGES, 5});
    }

    LoaderFactory BlueConfigLoader::createFactory() {
        return LoaderFactory(
            BLUE_CONFIG_LOADER_ID,
            BLUE_CONFIG_LOADER_NAME,
            true,
            [](const std::string& name) {
                return name == "BlueConfig";
            },
            [](LoaderFactory::FileProvider, const std::filesystem::path& path) {
                return LoaderFactory::FactoryResult(std::make_unique<BlueConfigLoader>(path));
            }
        );
    }
}

#endif
