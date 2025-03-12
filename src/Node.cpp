//
// Created by gaeqs on 09/03/2025.
//

#include <mnemea/Node.h>

#include <utility>

namespace mnemea {
    Node::Node(Node&& Other) noexcept
        : Identifiable(std::move(Other)),
          _type(std::move(Other._type)),
          _children(std::move(Other._children)),
          _neurons(std::move(Other._neurons)) {}

    Node& Node::operator=(Node&& Other) noexcept {
        if (this == &Other) return *this;
        Identifiable::operator =(std::move(Other));
        _type = std::move(Other._type);
        _children = std::move(Other._children);
        _neurons = std::move(Other._neurons);
        return *this;
    }

    Node::Node(UID uid, std::string type): Identifiable(uid), _type(std::move(type)) {}

    Result<Node*, NodeCreateError> Node::createNode(UID uid, std::string type) {
        auto it = _children.find(uid);
        if (it != _children.end()) return NodeCreateError::ALREADY_EXISTS;
        auto [result, ok] = _children.insert({uid, std::make_unique<Node>(uid, type)});
        if (!ok) return NodeCreateError::ERROR_WHILE_CREATING;
        return result->second.get();
    }

    Result<Node*, NodeCreateError> Node::getOrCreateNode(UID uid, std::string type) {
        auto it = _children.find(uid);
        if (it != _children.end()) return it->second.get();
        auto [result, ok] = _children.insert({uid, std::make_unique<Node>(uid, type)});
        if (!ok) return NodeCreateError::ERROR_WHILE_CREATING;
        return result->second.get();
    }

    std::optional<const Node*> Node::getNode(UID uid) const {
        auto it = _children.find(uid);
        if (it != _children.end()) return {};
        return it->second.get();
    }

    bool Node::addNeuron(Neuron* neuron, bool override) {
        if (override) {
            _neurons.insert_or_assign(neuron->getUID(), neuron);
            return true;
        }
        return _neurons.insert({neuron->getUID(), neuron}).second;
    }

    std::optional<Neuron*> Node::getNeuron(UID uid) {
        auto it = _neurons.find(uid);
        if (it != _neurons.end()) return {};
        return it->second;
    }

    std::optional<const Neuron*> Node::getNeuron(UID uid) const {
        auto it = _neurons.find(uid);
        if (it != _neurons.end()) return {};
        return it->second;
    }

    std::optional<Node*> Node::getNode(UID uid) {
        auto it = _children.find(uid);
        if (it != _children.end()) return {};
        return it->second.get();
    }
}
