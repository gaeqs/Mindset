//
// Created by gaeqs on 11/03/2025.
//

#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <span>
#include <ranges>
#include <vector>

#include <mindset/Synapse.h>
#include <mindset/Versioned.h>

namespace mindset
{
    /**
     * The Circuit class represents the neural connectivity within a dataset.
     *
     * It manages synaptic connections, providing efficient access to pre- and post-synaptic relations.
     */
    class Circuit : public Versioned
    {
        std::vector<Synapse> _synapses;
        std::multimap<UID, size_t> _preSynapses;
        std::multimap<UID, size_t> _postSynapses;

      public:
        /**
         * Constructs an empty Circuit.
         */
        Circuit();

        /**
         * Adds a single synapse to the circuit.
         * @param synapse The synapse to add.
         */
        void addSynapse(Synapse synapse);

        /**
         * Adds multiple synapses to the circuit.
         * @param synapses A vector containing synapses to add.
         */
        void addSynapses(std::vector<Synapse> synapses);

        /**
         * Clears all synapses from the circuit.
         */
        void clear();

        /**
         * Gets a mutable view of all synapses in the circuit.
         * @return Span containing mutable synapses.
         */
        std::span<Synapse> getSynapses();

        /**
         * Gets a read-only view of all synapses in the circuit.
         * @return Span containing const synapses.
         */
        std::span<const Synapse> getSynapses() const;

        /**
         * Gets mutable synapses that have the given neuron as pre-synaptic.
         * @param uid Neuron UID.
         * @return Range view of mutable pre-synaptic synapses.
         */
        [[nodiscard]] auto getPreSynapses(UID uid)
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
        [[nodiscard]] auto getPreSynapses(UID uid) const
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
        [[nodiscard]] auto getPostSynapses(UID uid)
        {
            auto [begin, end] = _postSynapses.equal_range(uid);
            auto range = std::ranges::subrange(begin, end);
            return range | std::views::transform([&](const auto& pair) -> Synapse& { return _synapses[pair.second]; });
        }

        /**
         * Gets const synapses that have the given neuron as post-synaptic.
         * @param uid Neuron UID.
         * @return Range view of const post-synaptic synapses.
         */
        [[nodiscard]] auto getPostSynapses(UID uid) const
        {
            auto [begin, end] = _postSynapses.equal_range(uid);
            auto range = std::ranges::subrange(begin, end);
            return range |
                   std::views::transform([&](const auto& pair) -> const Synapse& { return _synapses[pair.second]; });
        }
    };
} // namespace mindset

#endif //CIRCUIT_H
