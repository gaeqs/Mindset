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

#ifndef NODE_H
#define NODE_H

#include <memory>
#include <string>
#include <optional>
#include <ranges>
#include <unordered_set>

#include <mindset/Identifiable.h>
#include <mindset/util/Result.h>

#include <mindset/Neuron.h>

namespace mindset
{
    class Node;

    enum class NodeCreateError
    {
        ALREADY_EXISTS,
        ERROR_WHILE_CREATING
    };

    /**
     * Represents a hierarchical node structure containing child nodes and associated neurons.
     */
    class Node : public Identifiable
    {
        std::string _type;
        std::unordered_map<UID, std::unique_ptr<Node>> _children;
        std::unordered_set<UID> _neurons;

      public:
        Node(const Node& other) = delete;

        Node(Node&& other) noexcept;

        Node& operator=(Node&& other) noexcept;

        /**
         * Constructs a Node with a unique identifier and type.
         * @param uid Unique identifier of the node.
         * @param type String representing the node type.
         */
        Node(UID uid, std::string type);

        /**
         * Creates a child node under this node.
         * @param uid UID for the new child node.
         * @param type Type of the child node.
         */
        [[nodiscard]] Result<Node*, NodeCreateError> createNode(UID uid, std::string type);

        /**
         * Retrieves or creates a child node.
         * @param uid UID for the node.
         * @param type Type of the node.
         */
        [[nodiscard]] Result<Node*, NodeCreateError> getOrCreateNode(UID uid, std::string type);

        /**
         * Retrieves a mutable child node, if it exists.
         */
        [[nodiscard]] std::optional<Node*> getNode(UID uid);

        /**
         * Retrieves a const child node, if it exists.
         */
        [[nodiscard]] std::optional<const Node*> getNode(UID uid) const;

        /**
         * Associates a neuron with this node.
         * @param neuron UID of the neuron.
         * @return True if the neuron was added; false if it already existed.
         */
        bool addNeuron(UID neuron);

        /**
         * Returns a mutable range view of child nodes.
         */
        [[nodiscard]] auto getNodes()
        {
            return _children | std::views::transform([](auto& pair) -> Node* { return pair.second; });
        }

        /**
         * Returns a const range view of child nodes.
         */
        [[nodiscard]] auto getNodes() const
        {
            return _children | std::views::transform([](const auto& pair) -> const Node* { return pair.second; });
        }

        /**
         * Returns a const range view of associated neuron UIDs.
         */
        [[nodiscard]] auto getNeurons() const
        {
            return _neurons | std::views::transform([](const UID pair) -> const UID { return pair; });
        }
    };
} // namespace mindset

#endif //NODE_H
