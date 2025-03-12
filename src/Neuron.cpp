//
// Created by gaeqs on 09/03/2025.
//

#include <mnemea/Neuron.h>

namespace mnemea {
    Neuron::Neuron(UID uid) : Identifiable(uid) {}

    std::optional<Neurite*> Neuron::getNeurite(UID uid) {
        auto it = _neurites.find(uid);
        if (it != _neurites.end()) return &it->second;
        return {};
    }

    std::optional<const Neurite*> Neuron::getNeurite(UID uid) const {
        auto it = _neurites.find(uid);
        if (it != _neurites.end()) return &it->second;
        return {};
    }

    void Neuron::reserveSpaceForNeurites(size_t amount) {
        _neurites.reserve(amount);
    }

    bool Neuron::addNeurite(Neurite neurite) {
        return _neurites.insert({neurite.getUID(), std::move(neurite)}).second;
    }

    size_t Neuron::getNeuritesAmount() const {
        return _neurites.size();
    }
}
