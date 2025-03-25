//
// Created by gaeqs on 11/03/2025.
//

#include <mnemea/Circuit.h>

namespace mnemea
{
    Circuit::Circuit() = default;

    void Circuit::addSynapse(Synapse synapse)
    {
        UID pre = synapse.getPreSynapticNeuron();
        UID post = synapse.getPostSynapticNeuron();
        _synapses.push_back(std::move(synapse));
        _preSynapses.insert({pre, _synapses.size()});
        _postSynapses.insert({post, _synapses.size()});
        setNewVersion();
    }

    void Circuit::addSynapses(std::vector<Synapse> synapses)
    {
        for (auto& synapse : synapses) {
            addSynapse(std::move(synapse));
        }
        setNewVersion();
    }

    void Circuit::clear()
    {
        _synapses.clear();
        _preSynapses.clear();
        _postSynapses.clear();
        setNewVersion();
    }

    std::span<Synapse> Circuit::getSynapses()
    {
        return _synapses;
    }

    std::span<const Synapse> Circuit::getSynapses() const
    {
        return _synapses;
    }
} // namespace mnemea
