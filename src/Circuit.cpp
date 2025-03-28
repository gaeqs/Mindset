//
// Created by gaeqs on 11/03/2025.
//

#include <unistd.h>
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

    void Circuit::clear()
    {
        _synapses.clear();
        _preSynapses.clear();
        _postSynapses.clear();
        incrementVersion();
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

} // namespace mindset
