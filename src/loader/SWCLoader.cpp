//
// Created by gaeqs on 13/03/25.
//

#include <mindset/loader/SWCLoader.h>

#include <fstream>
#include <mindset/DefaultProperties.h>

namespace mindset
{
    Result<SWCLoader::SWCSegment, std::string> SWCLoader::toSegment(size_t lineIndex) const
    {
        SWCSegment segment;

        std::stringstream ss(_lines[lineIndex]);
        ss >> segment.id;
        ss >> segment.type;
        ss >> segment.end.x();
        ss >> segment.end.y();
        ss >> segment.end.z();
        ss >> segment.radius;
        ss >> segment.parent;

        if (ss.fail()) {
            return "StringStream error while parsing segment " + std::to_string(lineIndex) +
                   ". Bits: " + std::to_string(ss.rdstate()) + ".";
        }

        return segment;
    }

    SWCLoader::SWCLoader(const LoaderCreateInfo& info, const std::vector<std::string>& lines) :
        Loader(info),
        _lines(lines)
    {
    }

    SWCLoader::SWCLoader(const LoaderCreateInfo& info, std::vector<std::string>&& lines) :
        Loader(info),
        _lines(std::move(lines))
    {
    }

    SWCLoader::SWCLoader(const LoaderCreateInfo& info, std::istream& stream) :
        Loader(info)
    {
        std::string line;
        while (std::getline(stream, line)) {
            _lines.push_back(std::move(line));
        }
    }

    SWCLoader::SWCLoader(const LoaderCreateInfo& info, const std::filesystem::path& path) :
        Loader(info)
    {
        std::ifstream stream(path);
        std::string line;
        while (std::getline(stream, line)) {
            _lines.push_back(std::move(line));
        }
    }

    void SWCLoader::addUIDProvider(std::function<UID()> provider)
    {
        _provider = provider;
    }

    Result<std::shared_ptr<Morphology>, std::string> SWCLoader::loadMorphology(Dataset& dataset) const
    {
        constexpr size_t STAGES = 3;

        std::unordered_map<UID, SWCSegment> prototypes;

        invoke({LoaderStatusType::LOADING, "Defining properties", STAGES, 0});

        // Define properties
        auto lock = dataset.writeLock();
        auto& properties = dataset.getProperties();
        auto propPosition = properties.defineProperty(PROPERTY_POSITION);
        auto propRadius = properties.defineProperty(PROPERTY_RADIUS);
        auto propParent = properties.defineProperty(PROPERTY_PARENT);
        auto propType = properties.defineProperty(PROPERTY_NEURITE_TYPE);
        lock.unlock();

        invoke({LoaderStatusType::LOADING, "Parsing SWC file", STAGES, 1});

        prototypes.reserve(_lines.size());
        for (size_t i = 0; i < _lines.size(); ++i) {
            auto line = _lines[i];
            if (line.starts_with("#") || line.empty()) {
                continue;
            }
            auto result = toSegment(i);
            if (!result.isOk()) {
                std::string error = "Error while converting segment " + std::to_string(i) + ". " + result.getError();
                invoke({LoaderStatusType::LOADING_ERROR, error, STAGES, 1});
                return error;
            }
            prototypes.emplace(result.getResult().id, result.getResult());
        }

        auto morphology = std::make_shared<Morphology>();
        morphology->reserveSpaceForNeurites(_lines.size());

        invoke({LoaderStatusType::LOADING, "Parsing neurites", STAGES, 2});

        std::optional<Soma> soma;
        std::unordered_set<UID> somaUIDs;
        // Check for somas
        for (auto& [id, prototype] : prototypes) {
            auto type = static_cast<NeuriteType>(prototype.type);
            if (type != NeuriteType::SOMA) {
                continue;
            }
            if (!soma.has_value()) {
                soma = Soma(id);
            }
            soma.value().addNode({prototype.end, prototype.radius});
            somaUIDs.insert(id);
        }

        rush::Sphere somaBB(soma->getCenter(), soma->getBestMeanRadius() * 1.2f);

        for (auto& [id, prototype] : prototypes) {
            if (intersects(somaBB, prototype.end)) {
                somaUIDs.insert(id);
            }
        }

        for (auto& [id, prototype] : prototypes) {
            auto type = static_cast<NeuriteType>(prototype.type);
            if (somaUIDs.contains(id)) {
                continue;
            }

            Neurite neurite(id);
            neurite.setProperty(propType, type);
            neurite.setProperty(propPosition, prototype.end);
            neurite.setProperty(propRadius, prototype.radius);
            if (prototype.parent >= 0) {
                UID parentUID = static_cast<UID>(prototype.parent);
                if (somaUIDs.contains(parentUID)) {
                    neurite.setProperty(propParent, soma.value().getUID());
                } else {
                    neurite.setProperty(propParent, parentUID);
                }
            }
            morphology->addNeurite(std::move(neurite));
        }

        if (soma.has_value()) {
            for (UID uid : somaUIDs) {
                if (uid != soma->getUID()) {
                    soma->addExtraId(uid);
                }
            }
            morphology->setSoma(std::move(soma.value()));
        }

        invoke({LoaderStatusType::DONE, "Done", STAGES, 3});

        return morphology;
    }

    void SWCLoader::load(Dataset& dataset) const
    {
        auto result = loadMorphology(dataset);
        if (!result.isOk()) {
            std::cerr << result.getError() << std::endl;
            return;
        }

        UID uid = _provider == nullptr ? 0 : _provider();
        auto lock = dataset.writeLock();
        if (auto neuron = dataset.getNeuron(uid)) {
            auto neuronLock = neuron.value()->writeLock();
            neuron.value()->setMorphology(result.getResult());
        } else {
            dataset.addNeuron(Neuron(uid, result.getResult()));
        }
    }

    LoaderFactory SWCLoader::createFactory()
    {
        return LoaderFactory(
            SWC_LOADER_ID, SWC_LOADER_NAME, false, {},
            [](const std::string& name) {
                std::string extension = std::filesystem::path(name).extension().string();
                return extension == ".swc";
            },
            [](const LoaderCreateInfo& info, const std::filesystem::path& path) {
                return FactoryResult(std::make_unique<SWCLoader>(info, path));
            },
            [](const LoaderCreateInfo& info, const std::vector<std::string>& lines) {
                return FactoryResult(std::make_unique<SWCLoader>(info, lines));
            },
            [](const LoaderCreateInfo& info, std::istream& stream) {
                return FactoryResult(std::make_unique<SWCLoader>(info, stream));
            });
    }
} // namespace mindset
