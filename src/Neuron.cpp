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

#include <mindset/Neuron.h>

#include <utility>

namespace mindset
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
        incrementVersion();
    }
} // namespace mindset
