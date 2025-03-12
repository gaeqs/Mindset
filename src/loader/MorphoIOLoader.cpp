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
    MorphoIOLoader::MorphoIOLoader(std::filesystem::path path) : _path(std::move(path)), _uid(0) {}

    UID MorphoIOLoader::getUID() const {
        return _uid;
    }

    void MorphoIOLoader::setUID(UID uid) {
        _uid = uid;
    }


    void MorphoIOLoader::load(Dataset& dataset) const {
        brain::neuron::Morphology morphology(brion::URI(absolute(_path).string()));

        auto& properties = dataset.getProperties();
        auto propPosition = properties.defineProperty(PROPERTY_POSITION);
        auto propRadius = properties.defineProperty(PROPERTY_RADIUS);
        auto propParent = properties.defineProperty(PROPERTY_PARENT);
        auto propType = properties.defineProperty(PROPERTY_NEURITE_TYPE);

        auto result = std::make_shared<Morphology>();
        auto& points = morphology.getPoints();
        auto& types = morphology.getSectionTypes();
        for (size_t i = 0; i < points.size(); ++i) {
            Neurite neurite(i);
            auto type = static_cast<NeuriteType>(types[i]);

            neurite.setPropertyAsAny(propPosition, rush::Vec3f(points[i]));
            neurite.setPropertyAsAny(propRadius, points[i].w / 2.0f); // Brion returns the diameter!
            neurite.setPropertyAsAny(propType, type);
            result->addNeurite(std::move(neurite));
        }

        auto& sections = morphology.getSections();
        for (auto& section: sections) {
            UID from = section.x;
            UID to = section.y;
            if (auto neurite = result->getNeurite(to); neurite.has_value()) {
                neurite.value()->setPropertyAsAny(propParent, from);
            }
        }

        dataset.addNeuron(Neuron(_uid, std::move(result)));
    }
}

#endif
