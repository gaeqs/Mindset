//
// Created by gaeqs on 09/03/2025.
//

#ifndef DATASET_H
#define DATASET_H

#include <optional>
#include <unordered_map>

#include <mnemea/Neuron.h>
#include <mnemea/Node.h>
#include <mnemea/UID.h>
#include <mnemea/Properties.h>
#include <mnemea/Circuit.h>

namespace mnemea {
    class Dataset {
        std::unordered_map<UID, Neuron> _neurons;
        Properties _properties;
        Circuit _circuit;
        std::optional<Node> _hierarchy;

    public:
        Dataset();

        void reserveSpaceForNeurons(size_t amount);

        std::pair<Neuron*, bool> addNeuron(Neuron neuron);

        bool removeNeuron(UID uid);

        [[nodiscard]] std::optional<Neuron*> getNeuron(UID uid);

        [[nodiscard]] std::optional<const Neuron*> getNeuron(UID uid) const;

        [[nodiscard]] Properties& getProperties();

        [[nodiscard]] const Properties& getProperties() const;

        [[nodiscard]] Circuit& getCircuit();

        [[nodiscard]] const Circuit& getCircuit() const;

        [[nodiscard]] std::optional<Node*> getHierarchy();

        [[nodiscard]] std::optional<const Node*> getHierarchy() const;

        Node* createHierarchy(UID uid, std::string type);

        [[nodiscard]] auto getNeurons() {
            return _neurons | std::views::transform([](auto& pair) -> Neuron& {
                return pair.second;
            });
        }

        [[nodiscard]] auto getNeurons() const {
            return _neurons | std::views::transform([](const auto& pair) -> const Neuron& {
                return pair.second;
            });
        }
    };
}

#endif //DATASET_H
