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

#ifndef SYNAPSE_H
#define SYNAPSE_H

#include "Identifiable.h"

#include <mindset/PropertyHolder.h>
#include <mindset/UID.h>

namespace mindset
{
    /**
     * Represents a synaptic connection between two neurons.
     * Each synapse has associated pre- and post-synaptic neurons.
     */
    class Synapse : public Identifiable, public PropertyHolder
    {
        UID _preSynapticNeuron;
        UID _postSynapticNeuron;

      public:
        /**
         * Constructs a synapse between specified pre- and post-synaptic neurons.
         * @param preSynapticNeuron UID of the pre-synaptic neuron.
         * @param postSynapticNeuron UID of the post-synaptic neuron.
         */
        Synapse(UID uid, UID preSynapticNeuron, UID postSynapticNeuron);

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
