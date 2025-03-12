//
// Created by gaeqs on 11/03/2025.
//

#ifndef SYNAPSE_H
#define SYNAPSE_H

#include <mnemea/PropertyHolder.h>
#include <mnemea/UID.h>

namespace mnemea {
    class Synapse : public PropertyHolder {
        UID _preSynapticNeuron;
        UID _postSynapticNeuron;

    public:
        Synapse(UID preSynapticNeuron, UID postSynapticNeuron);

        UID getPreSynapticNeuron() const;

        void setPreSynapticNeuron(UID uid);

        UID getPostSynapticNeuron() const;

        void setPostSynapticNeuron(UID uid);
    };
}

#endif //SYNAPSE_H
