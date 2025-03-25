//
// Created by gaeqs on 12/03/25.
//

#include <mnemea/Morphology.h>

namespace mnemea
{
    Morphology::Morphology() = default;

    std::optional<Soma*> Morphology::getSoma()
    {
        if (_soma.has_value()) {
            return &_soma.value();
        }
        return {};
    }

    std::optional<const Soma*> Morphology::getSoma() const
    {
        if (_soma.has_value()) {
            return &_soma.value();
        }
        return {};
    }

    void Morphology::setSoma(Soma soma)
    {
        setNewVersion();
        _soma = soma;
    }

    void Morphology::clearSoma()
    {
        if (!_soma.has_value()) {
            return;
        }
        setNewVersion();
        _soma = {};
    }

    std::optional<Neurite*> Morphology::getNeurite(UID uid)
    {
        auto it = _neurites.find(uid);
        if (it != _neurites.end()) {
            return &it->second;
        }
        return {};
    }

    std::optional<const Neurite*> Morphology::getNeurite(UID uid) const
    {
        auto it = _neurites.find(uid);
        if (it != _neurites.end()) {
            return &it->second;
        }
        return {};
    }

    void Morphology::reserveSpaceForNeurites(size_t amount)
    {
        _neurites.reserve(amount);
    }

    std::pair<Neurite*, bool> Morphology::addNeurite(Neurite neurite)
    {
        auto [it, result] = _neurites.insert({neurite.getUID(), std::move(neurite)});
        setNewVersion();
        return {&it->second, result};
    }

    size_t Morphology::getNeuritesAmount() const
    {
        return _neurites.size();
    }
} // namespace mnemea
