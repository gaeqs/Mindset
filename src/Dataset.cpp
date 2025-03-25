//
// Created by gaeqs on 09/03/2025.
//

#include <mnemea/Dataset.h>

namespace mnemea
{
    Dataset::Dataset()
    {
    }

    void Dataset::reserveSpaceForNeurons(size_t amount)
    {
        _neurons.reserve(amount);
    }

    std::pair<Neuron*, bool> Dataset::addNeuron(Neuron neuron)
    {
        auto [it, result] = _neurons.insert({neuron.getUID(), std::move(neuron)});
        setNewVersion();
        return {&it->second, result};
    }

    bool Dataset::removeNeuron(UID uid)
    {
        bool result = _neurons.erase(uid) > 0;
        if (result) {
            setNewVersion();
        }
        return result;
    }

    std::optional<Neuron*> Dataset::getNeuron(UID uid)
    {
        auto it = _neurons.find(uid);
        if (it != _neurons.end()) {
            return &it->second;
        }
        return {};
    }

    std::optional<const Neuron*> Dataset::getNeuron(UID uid) const
    {
        auto it = _neurons.find(uid);
        if (it != _neurons.end()) {
            return &it->second;
        }
        return {};
    }

    Properties& Dataset::getProperties()
    {
        return _properties;
    }

    const Properties& Dataset::getProperties() const
    {
        return _properties;
    }

    Circuit& Dataset::getCircuit()
    {
        return _circuit;
    }

    const Circuit& Dataset::getCircuit() const
    {
        return _circuit;
    }

    std::optional<Node*> Dataset::getHierarchy()
    {
        if (_hierarchy.has_value()) {
            return &_hierarchy.value();
        }
        return {};
    }

    std::optional<const Node*> Dataset::getHierarchy() const
    {
        if (_hierarchy.has_value()) {
            return &_hierarchy.value();
        }
        return {};
    }

    Node* Dataset::createHierarchy(UID uid, std::string type)
    {
        _hierarchy.emplace(uid, type);
        setNewVersion();
        return &_hierarchy.value();
    }
} // namespace mnemea
