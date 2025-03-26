//
// Created by gaeqs on 11/03/2025.
//

#ifndef SYNAPSE_H
#define SYNAPSE_H

#include <mindset/PropertyHolder.h>
#include <mindset/UID.h>

namespace mindset
{
    /**
     * Represents a synaptic connection between two neurons.
     * Each synapse has associated pre- and post-synaptic neurons.
     */
    class Synapse : public PropertyHolder
    {
        UID _preSynapticNeuron;
        UID _postSynapticNeuron;

      public:
        /**
         * Constructs a synapse between specified pre- and post-synaptic neurons.
         * @param preSynapticNeuron UID of the pre-synaptic neuron.
         * @param postSynapticNeuron UID of the post-synaptic neuron.
         */
        Synapse(UID preSynapticNeuron, UID postSynapticNeuron);

        /**
         * Retrieves the UID of the pre-synaptic neuron.
         */
        [[nodiscard]] UID getPreSynapticNeuron() const;

        /**
         * Sets the UID of the pre-synaptic neuron.
         * @param uid UID of the neuron.
         */
        void setPreSynapticNeuron(UID uid);

        /**
         * Retrieves the UID of the post-synaptic neuron.
         */
        [[nodiscard]] UID getPostSynapticNeuron() const;

        /**
         * Sets the UID of the post-synaptic neuron.
         * @param uid UID of the neuron.
         */
        void setPostSynapticNeuron(UID uid);
    };
} // namespace mindset

#endif //SYNAPSE_H
