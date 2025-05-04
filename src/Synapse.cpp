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

#include <mindset/Synapse.h>

namespace mindset
{
    Synapse::Synapse(UID uid, UID preSynapticNeuron, UID postSynapticNeuron) :
        Identifiable(uid),
        _preSynapticNeuron(preSynapticNeuron),
        _postSynapticNeuron(postSynapticNeuron)
    {
    }

    UID Synapse::getPreSynapticNeuron() const
    {
        return _preSynapticNeuron;
    }

    void Synapse::setPreSynapticNeuron(UID uid)
    {
        _preSynapticNeuron = uid;
    }

    UID Synapse::getPostSynapticNeuron() const
    {
        return _postSynapticNeuron;
    }

    void Synapse::setPostSynapticNeuron(UID uid)
    {
        _postSynapticNeuron = uid;
    }
} // namespace mindset
