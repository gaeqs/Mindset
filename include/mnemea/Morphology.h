//
// Created by gaeqs on 12/03/25.
//

#ifndef MORPHOLOGY_H
#define MORPHOLOGY_H

#include <ranges>
#include <mnemea/Neurite.h>

namespace mnemea {
    class Morphology {
        std::unordered_map<UID, Neurite> _neurites;

    public:
        [[nodiscard]] std::optional<Neurite*> getNeurite(UID uid);

        [[nodiscard]] std::optional<const Neurite*> getNeurite(UID uid) const;

        void reserveSpaceForNeurites(size_t amount);

        std::pair<Neurite*, bool> addNeurite(Neurite neurite);

        [[nodiscard]] size_t getNeuritesAmount() const;

        [[nodiscard]] auto getNeurites() {
            return _neurites | std::views::transform([](auto& pair) -> Neurite& {
                return pair.second;
            });
        }

        [[nodiscard]] auto getNeurites() const {
            return _neurites | std::views::transform([](const auto& pair) -> const Neurite& {
                return pair.second;
            });
        }
    };
}

#endif //MORPHOLOGY_H
