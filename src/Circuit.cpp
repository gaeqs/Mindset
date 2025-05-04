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

#include <mindset/Circuit.h>

namespace mindset
{
    Circuit::Circuit() = default;

    std::pair<Synapse*, bool> Circuit::addSynapse(Synapse synapse)
    {
        UID pre = synapse.getPreSynapticNeuron();
        UID post = synapse.getPostSynapticNeuron();
        UID uid = synapse.getUID();
        auto [it, result] = _synapses.insert({uid, std::move(synapse)});
        if (result) {
            _preSynapses.insert({pre, uid});
            _postSynapses.insert({post, uid});
            incrementVersion();
            _synapseAddedEvent.invoke(&it->second);
        }
        return {&it->second, result};
    }

    void Circuit::addSynapses(std::vector<Synapse> synapses)
    {
        for (auto& synapse : synapses) {
            addSynapse(std::move(synapse));
        }
        incrementVersion();
    }

    bool Circuit::removeSynapse(UID uid)
    {
        auto it = _synapses.find(uid);
        if (it == _synapses.end()) {
            return false;
        }

        UID pre = it->second.getPreSynapticNeuron();
        UID post = it->second.getPostSynapticNeuron();

        _synapses.erase(it);

        auto [preBegin, preEnd] = _preSynapses.equal_range(pre);
        for (auto iter = preBegin; iter != preEnd; ++iter) {
            if (iter->second == uid) {
                _preSynapses.erase(iter);
                break;
            }
        }

        auto [postBegin, postEnd] = _postSynapses.equal_range(post);
        for (auto iter = postBegin; iter != postEnd; ++iter) {
            if (iter->second == uid) {
                _postSynapses.erase(iter);
                break;
            }
        }

        _synapseRemovedEvent.invoke(uid);

        incrementVersion();
        return true;
    }

    void Circuit::clear()
    {
        _synapses.clear();
        _preSynapses.clear();
        _postSynapses.clear();
        incrementVersion();
        _clearEvent.invoke(nullptr);
    }

    std::optional<Synapse*> Circuit::getSynapse(UID uid)
    {
        auto it = _synapses.find(uid);
        if (it != _synapses.end()) {
            return &it->second;
        }
        return {};
    }

    std::optional<const Synapse*> Circuit::getSynapse(UID uid) const
    {
        auto it = _synapses.find(uid);
        if (it != _synapses.end()) {
            return &it->second;
        }
        return {};
    }

    hey::Observable<Synapse*>& Circuit::getSynapseAddedEvent()
    {
        return _synapseAddedEvent;
    }

    hey::Observable<UID>& Circuit::getSynapseRemovedEvent()
    {
        return _synapseRemovedEvent;
    }

    hey::Observable<void*>& Circuit::getClearEvent()
    {
        return _clearEvent;
    }

} // namespace mindset
