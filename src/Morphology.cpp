// Copyright (c) 2025. VG-Lab/URJC.
//
// Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
//
// This file is part of Mindset <https://gitlab.gmrv.es/g.rial/mindset>
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 3.0 as published
// by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include <mindset/Morphology.h>

namespace mindset
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
        incrementVersion();
        _soma = soma;
    }

    void Morphology::clearSoma()
    {
        if (!_soma.has_value()) {
            return;
        }
        incrementVersion();
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
        incrementVersion();
        return {&it->second, result};
    }
    bool Morphology::removeNeurite(UID uid)
    {
        auto result = _neurites.erase(uid) > 0;
        if (result) {
            incrementVersion();
        }
        return result;
    }

    size_t Morphology::getNeuritesAmount() const
    {
        return _neurites.size();
    }

    std::optional<MorphologyTree*> Morphology::getMorphologyTree()
    {
        if (_tree.has_value()) {
            return &_tree.value();
        }
        return {};
    }

    std::optional<const MorphologyTree*> Morphology::getMorphologyTree() const
    {
        if (_tree.has_value()) {
            return &_tree.value();
        }
        return {};
    }

    MorphologyTree* Morphology::getOrCreateMorphologyTree(const Dataset& dataset)
    {
        if (!_tree.has_value() || _tree.value().getMorphologyVersion() != getVersion()) {
            // Recompute tree
            _tree = MorphologyTree(this, dataset);
        }

        return &_tree.value();
    }

    void Morphology::setMorphologyTree(MorphologyTree tree)
    {
        _tree = std::move(tree);
    }
} // namespace mindset
