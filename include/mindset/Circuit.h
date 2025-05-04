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

#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <ranges>
#include <unordered_map>

#include <hey/Hey.h>

#include <mindset/Synapse.h>
#include <mindset/Versioned.h>
#include <mindset/MutexHolder.h>

namespace mindset
{
    /**
     * The Circuit class represents the neural connectivity within a dataset.
     *
     * It manages synaptic connections, providing efficient access to pre- and post-synaptic relations.
     */
    class Circuit : public Versioned, public MutexHolder
    {
        std::unordered_map<UID, Synapse> _synapses;
        std::unordered_multimap<UID, UID> _preSynapses;
        std::unordered_multimap<UID, UID> _postSynapses;

        hey::Observable<Synapse*> _synapseAddedEvent;
        hey::Observable<UID> _synapseRemovedEvent;
        hey::Observable<void*> _clearEvent;

      public:
        /**
         * Constructs an empty Circuit.
         */
        Circuit();

        /**
         * Adds a single synapse to the circuit.
         * @param synapse The synapse to add.
         * @return Pair containing a pointer to the inserted synapse and a boolean indicating success.
         */
        std::pair<Synapse*, bool> addSynapse(Synapse synapse);

        /**
         * Adds multiple synapses to the circuit.
         * @param synapses A vector containing synapses to add.
         */
        void addSynapses(std::vector<Synapse> synapses);

        /**
         * Removes a synapse identified by its UID.
         * @param uid The unique identifier of the synapse.
         * @return True if the synapse was successfully removed; false otherwise.
         */
        bool removeSynapse(UID uid);

        /**
         * Clears all synapses from the circuit.
         */
        void clear();

        /**
         * Returns a mutable pointer to the synapse with the specified UID.
         */
        [[nodiscard]] std::optional<Synapse*> getSynapse(UID uid);

        /**
         * Returns a const pointer to the synapses with the specified UID.
         */
        [[nodiscard]] std::optional<const Synapse*> getSynapse(UID uid) const;

        /**
         * The observable that manages the event triggered when a synapse is added.
         */
        hey::Observable<Synapse*>& getSynapseAddedEvent();

        /**
         * The observable that manages the event triggered when a synapse is removed.
         */
        hey::Observable<UID>& getSynapseRemovedEvent();

        /**
         * The observable that manages the event triggered when the dataset is cleared.
         */
        hey::Observable<void*>& getClearEvent();

        /**
         * Returns a view to iterate over all stored synapses' UIDs.
         * @returns A range view of UIDs.
         */
        [[nodiscard]] decltype(auto) getSynapsesUIDs() const
        {
            return _synapses | std::views::keys;
        }

        /**
         * Returns a view to iterate over all stored synapses in a mutable context.
         * @return A range view of mutable synapses references.
         */
        [[nodiscard]] decltype(auto) getSynapses()
        {
            return _synapses | std::views::transform([](auto& pair) -> Synapse* { return &pair.second; });
        }

        /**
         * Returns a view to iterate over all stored synapses in a const context.
         * @return A range view of const synapses references.
         */
        [[nodiscard]] decltype(auto) getSynapses() const
        {
            return _synapses | std::views::transform([](const auto& pair) -> const Synapse* { return &pair.second; });
        }

        /**
         * Gets mutable synapses that have the given neuron as pre-synaptic.
         * @param uid Neuron UID.
         * @return Range view of mutable pre-synaptic synapses.
         */
        [[nodiscard]] decltype(auto) getPreSynapses(UID uid)
        {
            auto [begin, end] = _preSynapses.equal_range(uid);
            auto range = std::ranges::subrange(begin, end);
            return range | std::views::transform([&](const auto& pair) -> Synapse& { return _synapses[pair.second]; });
        }

        /**
         * Gets const synapses that have the given neuron as pre-synaptic.
         * @param uid Neuron UID.
         * @return Range view of const pre-synaptic synapses.
         */
        [[nodiscard]] decltype(auto) getPreSynapses(UID uid) const
        {
            auto [begin, end] = _preSynapses.equal_range(uid);
            auto range = std::ranges::subrange(begin, end);
            return range |
                   std::views::transform([&](const auto& pair) -> const Synapse& { return _synapses[pair.second]; });
        }

        /**
         * Gets mutable synapses that have the given neuron as post-synaptic.
         * @param uid Neuron UID.
         * @return Range view of mutable post-synaptic synapses.
         */
        [[nodiscard]] decltype(auto) getPostSynapses(UID uid)
        {
            auto [begin, end] = _postSynapses.equal_range(uid);
            auto range = std::ranges::subrange(begin, end);
            return range | std::views::transform([&](const auto& pair) -> Synapse* { return &_synapses[pair.second]; });
        }

        /**
         * Gets const synapses that have the given neuron as post-synaptic.
         * @param uid Neuron UID.
         * @return Range view of const post-synaptic synapses.
         */
        [[nodiscard]] decltype(auto) getPostSynapses(UID uid) const
        {
            auto [begin, end] = _postSynapses.equal_range(uid);
            auto range = std::ranges::subrange(begin, end);
            return range |
                   std::views::transform([&](const auto& pair) -> const Synapse* { return &_synapses[pair.second]; });
        }
    };
} // namespace mindset

#endif // CIRCUIT_H
