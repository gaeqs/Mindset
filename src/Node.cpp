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

#include <mindset/Node.h>

#include <utility>

namespace mindset
{
    Node::Node(Node&& Other) noexcept :
        Identifiable(std::move(Other)),
        _type(std::move(Other._type)),
        _children(std::move(Other._children)),
        _neurons(std::move(Other._neurons))
    {
    }

    Node& Node::operator=(Node&& Other) noexcept
    {
        if (this == &Other) {
            return *this;
        }
        Identifiable::operator=(std::move(Other));
        _type = std::move(Other._type);
        _children = std::move(Other._children);
        _neurons = std::move(Other._neurons);
        return *this;
    }

    Node::Node(UID uid, std::string type) :
        Identifiable(uid),
        _type(std::move(type))
    {
    }

    Result<Node*, NodeCreateError> Node::createNode(UID uid, std::string type)
    {
        auto it = _children.find(uid);
        if (it != _children.end()) {
            return NodeCreateError::ALREADY_EXISTS;
        }
        auto [result, ok] = _children.insert({uid, std::make_unique<Node>(uid, type)});
        if (!ok) {
            return NodeCreateError::ERROR_WHILE_CREATING;
        }
        return result->second.get();
    }

    Result<Node*, NodeCreateError> Node::getOrCreateNode(UID uid, std::string type)
    {
        auto it = _children.find(uid);
        if (it != _children.end()) {
            return it->second.get();
        }
        auto [result, ok] = _children.insert({uid, std::make_unique<Node>(uid, type)});
        if (!ok) {
            return NodeCreateError::ERROR_WHILE_CREATING;
        }
        return result->second.get();
    }

    std::optional<const Node*> Node::getNode(UID uid) const
    {
        auto it = _children.find(uid);
        if (it != _children.end()) {
            return {};
        }
        return it->second.get();
    }

    bool Node::addNeuron(UID neuron)
    {
        return _neurons.insert(neuron).second;
    }

    std::optional<Node*> Node::getNode(UID uid)
    {
        auto it = _children.find(uid);
        if (it != _children.end()) {
            return {};
        }
        return it->second.get();
    }
} // namespace mindset
