//
// Created by gaeqs on 09/03/2025.
//

#ifndef NEURON_H
#define NEURON_H

#include <ranges>
#include <mnemea/Identifiable.h>
#include <mnemea/PropertyHolder.h>
#include <mnemea/Neurite.h>

namespace mnemea {
    class Neuron : public Identifiable, public PropertyHolder {
        std::unordered_map<UID, Neurite> _neurites;

    public:
        explicit Neuron(UID uid);

        [[nodiscard]] std::optional<Neurite*> getNeurite(UID uid);

        [[nodiscard]] std::optional<const Neurite*> getNeurite(UID uid) const;

        void reserveSpaceForNeurites(size_t amount);

        bool addNeurite(Neurite neurite);

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


#endif //NEURON_H
