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
        if (_loadHierarchy) {
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

        auto root = dataset.createHierarchy(0, "mnemea:root");

        size_t index = 0;
        for (UID id: ids) {
            if (auto neuron = dataset.getNeuron(id); neuron.has_value()) {
                auto sub = data[index];

                UID column = boost::lexical_cast<UID>(sub[0]);
                UID miniColumn = boost::lexical_cast<UID>(sub[1]);

                if (auto columnResult = root->getOrCreateNode(column, "mnemea:column"); columnResult.isOk()) {
                    if (auto miniColumnResult = columnResult.getResult()->getOrCreateNode(miniColumn,
                        "mnemea:mini_column"); miniColumnResult.isOk()) {
                        miniColumnResult.getResult()->addNeuron(neuron.value());
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
        auto& types = morphology.getSectionTypes();
        for (size_t i = 0; i < points.size(); ++i) {
            Neurite neurite(i);
            auto type = static_cast<NeuriteType>(types[i]);

            neurite.setPropertyAsAny(properties.neuritePosition, rush::Vec3f(points[i]));
            neurite.setPropertyAsAny(properties.neuriteRadius, points[i].w / 2.0f); // Brion returns the diameter!
            neurite.setPropertyAsAny(properties.neuriteType, type);
            result->addNeurite(std::move(neurite));
        }

        auto& sections = morphology.getSections();
        for (auto& section: sections) {
            UID from = section.x;
            UID to = section.y;
            if (auto neurite = result->getNeurite(to); neurite.has_value()) {
                neurite.value()->setPropertyAsAny(properties.neuriteParent, from);
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
        if (_targets.empty()) return;


        brion::GIDSet ids;
        for (auto& target: _targets) {
            brion::GIDSet targetSet = _blueConfig.parseTarget(target);
            ids.insert(targetSet.begin(), targetSet.end());
        }
        if (ids.empty()) return;
        BlueConfigLoaderProperties properties = initProperties(dataset.getProperties());

        /**/ {
            auto circuit = brain::Circuit(_blueConfig);
            loadNeurons(dataset, properties, ids, circuit);

            if (_loadMorphology) {
                loadMorphologies(dataset, properties, ids, circuit);
            }
        }

        if (_loadHierarchy) {
            auto circuit = brion::Circuit(_blueConfig.getCircuitSource());
            loadHierarchy(dataset, properties, ids, circuit);
        }
    }
}

#endif
