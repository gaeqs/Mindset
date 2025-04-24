//
// Created by gaeqs on 13/03/25.
//

#include <fstream>

#include <mindset/DefaultProperties.h>
#include <mindset/loader/XMLLoader.h>
#include <mindset/loader/SWCLoader.h>

namespace
{
    std::optional<mindset::UID> asUID(pugi::xml_attribute attr)
    {
        constexpr mindset::UID NOT_VALID = -1;
        mindset::UID id = attr.as_uint(NOT_VALID);
        if (id == NOT_VALID) {
            return {};
        }
        return id;
    }

    std::optional<std::string> asString(pugi::xml_attribute attr)
    {
        if (attr.empty()) {
            return {};
        }
        return attr.as_string();
    }

    mindset::Result<std::vector<float>, std::string> split(const std::string& string, char delimiter)
    {
        std::vector<float> tokens;
        std::stringstream ss(string);

        std::string token;
        while (std::getline(ss, token, delimiter)) {
            try {
                tokens.push_back(std::stof(token));
            } catch (const std::invalid_argument& e) {
                return {"Invalid float! " + std::string(token)};
            } catch (const std::out_of_range& e) {
                return {"Number out of range! " + std::string(token)};
            }
        }

        return tokens;
    }

    mindset::Result<std::vector<mindset::UID>, std::string> splitUID(const std::string& string, char delimiter)
    {
        std::vector<mindset::UID> tokens;
        std::stringstream ss(string);

        std::string token;
        while (std::getline(ss, token, delimiter)) {
            try {
                tokens.push_back(std::stoi(token));
            } catch (const std::invalid_argument& e) {
                return {"Invalid int! " + std::string(token)};
            } catch (const std::out_of_range& e) {
                return {"Number out of range! " + std::string(token)};
            }
        }

        return tokens;
    }
} // namespace

namespace mindset
{
    XMLLoader::XMLLoader(const LoaderCreateInfo& info, const void* data, size_t size) :
        Loader(info)
    {
        auto result = _doc.load_buffer(data, size);
        _valid = result.status == pugi::status_ok;
    }

    XMLLoader::XMLLoader(const LoaderCreateInfo& info, std::istream& stream) :
        Loader(info)
    {
        auto result = _doc.load(stream);
        _valid = result.status == pugi::status_ok;
    }

    XMLLoader::XMLLoader(const LoaderCreateInfo& info, std::filesystem::path path) :
        Loader(info)
    {
        std::ifstream stream(path);
        auto result = _doc.load(stream);
        _valid = result.status == pugi::status_ok;
    }

    void XMLLoader::load(Dataset& dataset) const
    {
        constexpr size_t STAGES = 3;
        invoke({LoaderStatusType::LOADING, "Validating XML file", STAGES, 0});

        if (!_valid) {
            std::cerr << "Parser is not valid" << std::endl;
            invoke({LoaderStatusType::LOADING_ERROR, "Parser is not valid", STAGES, 0});
            return;
        }
        if (!getFileProvider()) {
            std::cerr << "Filesystem is not set" << std::endl;
            invoke({LoaderStatusType::LOADING_ERROR, "Filesystem is not set", STAGES, 0});
            return;
        }
        auto scene = _doc.child("scene").child("morphology");
        if (!scene) {
            std::cerr << "Scene not found" << std::endl;
            invoke({LoaderStatusType::LOADING_ERROR, "Scene not found", STAGES, 0});
            return;
        };

        UID transformProp = dataset.getProperties().defineProperty(PROPERTY_TRANSFORM);
        std::unordered_map<UID, XMLNeuron> xmlNeurons;

        invoke({LoaderStatusType::LOADING, "Loading hierarchy", STAGES, 1});

        Node* root = dataset.getHierarchy().value_or(nullptr);
        if (root == nullptr) {
            root = dataset.createHierarchy(0, "mindset:root");
        }

        for (auto column : scene.child("columns").children("column")) {
            auto columnId = asUID(column.attribute("id"));

            Node* columnNode = nullptr;
            if (columnId.has_value()) {
                if (auto result = root->getOrCreateNode(columnId.value(), "mindset:column"); result.isOk()) {
                    columnNode = result.getResult();
                }
            }

            for (auto miniColumn : column.children("minicolumn")) {
                auto miniColumnId = asUID(miniColumn.attribute("id"));

                Node* miniColumnNode = nullptr;
                if (columnNode != nullptr && columnId.has_value()) {
                    auto result = columnNode->getOrCreateNode(columnId.value(), "mindset:mini_column");
                    if (result.isOk()) {
                        miniColumnNode = result.getResult();
                    }
                }

                for (auto neuron : miniColumn.children("neuron")) {
                    auto gid = asUID(neuron.attribute("gid"));
                    if (!gid.has_value()) {
                        std::cerr << "Neuron GID not found!" << std::endl;
                        invoke({LoaderStatusType::LOADING_ERROR, "Neuron GID not found", STAGES, 1});
                        return;
                    };

                    XMLNeuron xmlNeuron = {.id = gid.value(),
                                           .column = columnId,
                                           .miniColumn = miniColumnId,
                                           .layer = asUID(neuron.append_attribute("layer")),
                                           .neuronType = asString(neuron.attribute("type")),
                                           .transform = {},
                                           .node = miniColumnNode};

                    if (auto transform = neuron.child("transform").first_child()) {
                        std::string string = transform.value();
                        auto result = split(string, ',');
                        if (!result.isOk()) {
                            std::cerr << result.getError() << std::endl;
                            invoke({LoaderStatusType::LOADING_ERROR, result.getError(), STAGES, 1});
                            return;
                        };
                        auto floats = std::move(result.getResult());
                        if (floats.size() != 16) {
                            std::cerr << "Invalid matrix size." << std::endl;
                            invoke({LoaderStatusType::LOADING_ERROR, "Invalid matrix size", STAGES, 1});
                            return;
                        };

                        rush::Mat4f matrix([&floats](size_t c, size_t r) { return floats[r * 4 + c]; });

                        xmlNeuron.transform = NeuronTransform(matrix);
                    }

                    xmlNeurons.insert({gid.value(), std::move(xmlNeuron)});
                }
            }
        }

        invoke({LoaderStatusType::LOADING, "Load morphology", STAGES, 2});

        for (auto morpho : scene.child("neuronmorphologies").children("neuronmorphology")) {
            auto att = morpho.attribute("neurons");
            if (att.empty()) {
                continue;
            }

            auto fileAtt = morpho.attribute("swc");
            if (fileAtt.empty()) {
                continue;
            }

            auto result = splitUID(att.as_string(""), ',');
            if (!result.isOk()) {
                std::cerr << result.getError() << std::endl;
            }
            auto uids = std::move(result.getResult());
            if (uids.empty()) {
                continue;
            }

            auto fileName = std::string(fileAtt.as_string(""));
            auto lines = getFileProvider()(fileName);

            if (!lines.has_value()) {
                std::cerr << "File not found: " << fileName << std::endl;
            };

            auto loader = SWCLoader(LoaderCreateInfo(), lines.value());
            auto swcResult = loader.loadMorphology(dataset);
            if (!swcResult.isOk()) {
                auto error = "Error loading SWC file '" + fileName + "': " + swcResult.getError();
                std::cerr << error << std::endl;
                invoke({LoaderStatusType::LOADING_ERROR, error, STAGES, 2});
                return;
            };

            auto swc = std::move(swcResult.getResult());

            for (UID id : uids) {
                auto it = xmlNeurons.find(id);
                if (it == xmlNeurons.end()) {
                    continue;
                }
                auto& xml = it->second;

                if (auto presentNeuron = dataset.getNeuron(xml.id)) {
                    presentNeuron.value()->setMorphology(swc);
                    if (xml.transform.has_value()) {
                        presentNeuron.value()->setProperty(transformProp, xml.transform.value());
                    }
                    if (xml.node != nullptr) {
                        xml.node->addNeuron(xml.id);
                    }
                } else {
                    Neuron neuron(xml.id, swc);
                    if (xml.transform.has_value()) {
                        neuron.setProperty(transformProp, xml.transform.value());
                    }

                    auto [neuronInDataset, result] = dataset.addNeuron(std::move(neuron));
                    if (result && xml.node != nullptr) {
                        xml.node->addNeuron(neuronInDataset->getUID());
                    }
                }
            }
        }

        invoke({LoaderStatusType::DONE, "Done", STAGES, 3});
    }

    LoaderFactory XMLLoader::createFactory()
    {
        return LoaderFactory(
            SWC_LOADER_ID, SWC_LOADER_NAME, true, {},
            [](const std::string& name) {
                std::string extension = std::filesystem::path(name).extension().string();
                return extension == ".xml";
            },
            [](const LoaderCreateInfo& info, const std::filesystem::path& path) {
                return FactoryResult(std::make_unique<XMLLoader>(info, path));
            },
            nullptr,
            [](const LoaderCreateInfo& info, std::istream& stream) {
                return FactoryResult(std::make_unique<XMLLoader>(info, stream));
            });
    }
} // namespace mindset
