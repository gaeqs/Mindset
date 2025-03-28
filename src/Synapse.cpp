//
// Created by gaeqs on 11/03/2025.
//

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
