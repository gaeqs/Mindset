//
// Created by gaeqs on 12/03/25.
//

#include <mnemea/Morphology.h>

namespace mnemea {
    std::optional<Neurite*> Morphology::getNeurite(UID uid) {
        auto it = _neurites.find(uid);
        if (it != _neurites.end()) return &it->second;
        return {};
    }

    std::optional<const Neurite*> Morphology::getNeurite(UID uid) const {
        auto it = _neurites.find(uid);
        if (it != _neurites.end()) return &it->second;
        return {};
    }

    void Morphology::reserveSpaceForNeurites(size_t amount) {
        _neurites.reserve(amount);
    }

    bool Morphology::addNeurite(Neurite neurite) {
        return _neurites.insert({neurite.getUID(), std::move(neurite)}).second;
    }

    size_t Morphology::getNeuritesAmount() const {
        return _neurites.size();
    }
}
