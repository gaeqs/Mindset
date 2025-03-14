//
// Created by gaeqs on 09/03/2025.
//

#ifndef NODE_H
#define NODE_H

#include <memory>
#include <string>
#include <optional>
#include <ranges>
#include <unordered_set>

#include <mnemea/Identifiable.h>
#include <mnemea/util/Result.h>

#include <mnemea/Neuron.h>

namespace mnemea {
    class Node;

    enum class NodeCreateError {
        ALREADY_EXISTS,
        ERROR_WHILE_CREATING
    };

    class Node : public Identifiable {
        std::string _type;
        std::unordered_map<UID, std::unique_ptr<Node>> _children;
        std::unordered_set<UID> _neurons;

    public:
        Node(const Node& other) = delete;

        Node(Node&& Other) noexcept;

        Node& operator=(Node&& Other) noexcept;

        Node(UID uid, std::string type);

        [[nodiscard]] Result<Node*, NodeCreateError> createNode(UID uid, std::string type);

        [[nodiscard]] Result<Node*, NodeCreateError> getOrCreateNode(UID uid, std::string type);

        [[nodiscard]] std::optional<Node*> getNode(UID uid);

        [[nodiscard]] std::optional<const Node*> getNode(UID uid) const;

        bool addNeuron(UID neuron);

        [[nodiscard]] auto getNodes() {
            return _children | std::views::transform([](auto& pair) -> Node* {
                return pair.second;
            });
        }

        [[nodiscard]] auto getNodes() const {
            return _children | std::views::transform([](const auto& pair) -> const Node* {
                return pair.second;
            });
        }

        [[nodiscard]] auto getNeurons() const {
            return _neurons | std::views::transform([](const UID pair) -> const UID {
                return pair;
            });
        }
    };
}

#endif //NODE_H
