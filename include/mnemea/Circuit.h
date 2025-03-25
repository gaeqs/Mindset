//
// Created by gaeqs on 11/03/2025.
//

#ifndef CIRCUIT_H
#define CIRCUIT_H

#include <span>
#include <ranges>
#include <vector>

#include <mnemea/Synapse.h>
#include <mnemea/Versioned.h>

namespace mnemea
{
    class Circuit : public Versioned
    {
        std::vector<Synapse> _synapses;
        std::multimap<UID, size_t> _preSynapses;
        std::multimap<UID, size_t> _postSynapses;

      public:
        Circuit();

        void addSynapse(Synapse synapse);

        void addSynapses(std::vector<Synapse> synapses);

        void clear();

        std::span<Synapse> getSynapses();

        std::span<const Synapse> getSynapses() const;

        [[nodiscard]] auto getPreSynapses(UID uid)
        {
            auto [begin, end] = _preSynapses.equal_range(uid);
            auto range = std::ranges::subrange(begin, end);
            return range | std::views::transform([&](const auto& pair) -> Synapse& { return _synapses[pair.second]; });
        }

        [[nodiscard]] auto getPreSynapses(UID uid) const
        {
            auto [begin, end] = _preSynapses.equal_range(uid);
            auto range = std::ranges::subrange(begin, end);
            return range |
                   std::views::transform([&](const auto& pair) -> const Synapse& { return _synapses[pair.second]; });
        }

        [[nodiscard]] auto getPostSynapses(UID uid)
        {
            auto [begin, end] = _postSynapses.equal_range(uid);
            auto range = std::ranges::subrange(begin, end);
            return range | std::views::transform([&](const auto& pair) -> Synapse& { return _synapses[pair.second]; });
        }

        [[nodiscard]] auto getPostSynapses(UID uid) const
        {
            auto [begin, end] = _postSynapses.equal_range(uid);
            auto range = std::ranges::subrange(begin, end);
            return range |
                   std::views::transform([&](const auto& pair) -> const Synapse& { return _synapses[pair.second]; });
        }
    };
} // namespace mnemea

#endif //CIRCUIT_H
