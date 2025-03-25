//
// Created by gaeqs on 09/03/2025.
//

#include <mnemea/Neuron.h>

#include <utility>

namespace mnemea
{
    Neuron::Neuron(UID uid, std::shared_ptr<Morphology> morphology) :
        Identifiable(uid),
        _morphology(std::move(morphology))
    {
    }

    std::optional<Morphology*> Neuron::getMorphology()
    {
        if (_morphology == nullptr) {
            return {};
        }
        return _morphology.get();
    }

    std::optional<const Morphology*> Neuron::getMorphology() const
    {
        if (_morphology == nullptr) {
            return {};
        }
        return _morphology.get();
    }

    void Neuron::setMorphology(std::shared_ptr<Morphology> morphology)
    {
        _morphology = std::move(morphology);
        setNewVersion();
    }
} // namespace mnemea
