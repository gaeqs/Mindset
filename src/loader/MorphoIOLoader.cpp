//
// Created by gaeqs on 12/03/25.
//

#ifdef MNEMEA_BRION

#include <mnemea/loader/MorphoIOLoader.h>
#include <mnemea/DefaultProperties.h>

#include <brain/neuron/morphology.h>
#include <brain/circuit.h>
#include <rush/rush.h>


namespace mnemea {
    MorphoIOLoader::MorphoIOLoader(std::filesystem::path path) : _path(std::move(path)) {}

    void MorphoIOLoader::addUIDProvider(std::function<UID()> provider) {
        _provider = provider;
    }

    void MorphoIOLoader::load(Dataset& dataset) const {
        constexpr size_t STAGES = 5;
        invoke({LoaderStatusType::LOADING, "Loading morphology", STAGES, 0});
        brain::neuron::Morphology morphology(brion::URI(absolute(_path).string()));
        auto& points = morphology.getPoints();
        auto& sections = morphology.getSections();
        auto& types = morphology.getSectionTypes();

        invoke({LoaderStatusType::LOADING, "Defining properties", STAGES, 1});

        auto& properties = dataset.getProperties();
        auto propPosition = properties.defineProperty(PROPERTY_POSITION);
        auto propRadius = properties.defineProperty(PROPERTY_RADIUS);
        auto propParent = properties.defineProperty(PROPERTY_PARENT);
        auto propType = properties.defineProperty(PROPERTY_NEURITE_TYPE);

        invoke({LoaderStatusType::LOADING, "Parsing neurites", STAGES, 2});

        auto result = std::make_shared<Morphology>();
        for (size_t i = 0; i < points.size(); ++i) {
            Neurite neurite(i);
            auto type = static_cast<NeuriteType>(types[i]);

            neurite.setPropertyAsAny(propPosition, rush::Vec3f(points[i]));
            neurite.setPropertyAsAny(propRadius, points[i].w / 2.0f); // Brion returns the diameter!
            neurite.setPropertyAsAny(propType, type);
            result->addNeurite(std::move(neurite));
        }

        invoke({LoaderStatusType::LOADING, "Parsing hierarchy", STAGES, 3});

        for (auto& section: sections) {
            UID from = section.x;
            UID to = section.y;
            if (auto neurite = result->getNeurite(to); neurite.has_value()) {
                neurite.value()->setPropertyAsAny(propParent, from);
            }
        }

        invoke({LoaderStatusType::LOADING, "Creating neuron", STAGES, 4});
        dataset.addNeuron(Neuron(_provider == nullptr ? 0 : _provider(), std::move(result)));
        invoke({LoaderStatusType::DONE, "Done", STAGES, 5});
    }

    LoaderFactory MorphoIOLoader::createFactory() {
        return LoaderFactory(
            MORPHO_IO_LOADER_ID,
            MORPHO_IO_LOADER_NAME,
            false,
            [](const std::string& name) {
                std::string extension = std::filesystem::path(name).extension().string();
                return extension == ".h5" || extension == ".swc" || extension == ".arc" || extension == ".hdf5";
            },
            [](LoaderFactory::FileProvider, const std::filesystem::path& path) {
                return LoaderFactory::Result(std::make_unique<MorphoIOLoader>(path));
            }
        );
    }
}

#endif
