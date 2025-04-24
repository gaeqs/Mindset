//
// Created by gaeqs on 09/03/2025.
//

#include <mindset/Dataset.h>

namespace mindset
{
    Dataset::Dataset()
    {
    }

    void Dataset::reserveSpaceForNeurons(size_t amount)
    {
        _neurons.reserve(amount);
    }

    size_t Dataset::getNeuronsAmount() const
    {
        return _neurons.size();
    }

    std::pair<Neuron*, bool> Dataset::addNeuron(Neuron neuron)
    {
        auto [it, result] = _neurons.insert({neuron.getUID(), std::move(neuron)});
        if (result) {
            _neuronAddedEvent.invoke(&it->second);
            incrementVersion();
        }
        return {&it->second, result};
    }

    bool Dataset::removeNeuron(UID uid)
    {
        bool result = _neurons.erase(uid) > 0;
        if (result) {
            _neuronRemovedEvent.invoke(uid);
            incrementVersion();
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
        incrementVersion();
        return &_hierarchy.value();
    }

    size_t Dataset::getActivitiesAmount() const
    {
        return _activities.size();
    }

    std::pair<Activity*, bool> Dataset::addActivity(Activity activity)
    {
        auto [it, result] = _activities.insert({activity.getUID(), std::move(activity)});
        if (result) {
            _activityAddedEvent.invoke(&it->second);
            incrementVersion();
        }
        return {&it->second, result};
    }

    bool Dataset::removeActivity(UID uid)
    {
        bool result = _activities.erase(uid) > 0;
        if (result) {
            _activityRemovedEvent.invoke(uid);
            incrementVersion();
        }
        return result;
    }

    std::optional<Activity*> Dataset::getActivity(UID uid)
    {
        auto it = _activities.find(uid);
        if (it != _activities.end()) {
            return &it->second;
        }
        return {};
    }

    std::optional<const Activity*> Dataset::getActivity(UID uid) const
    {
        auto it = _activities.find(uid);
        if (it != _activities.end()) {
            return &it->second;
        }
        return {};
    }

    void Dataset::clear()
    {
        _neurons.clear();
        _circuit.clear();
        _hierarchy = {};
        _activities.clear();
        _clearEvent.invoke(nullptr);
        incrementVersion();
    }

    hey::Observable<Neuron*>& Dataset::getNeuronAddedEvent()
    {
        return _neuronAddedEvent;
    }

    hey::Observable<UID>& Dataset::getNeuronRemovedEvent()
    {
        return _neuronRemovedEvent;
    }

    hey::Observable<void*>& Dataset::getClearEvent()
    {
        return _clearEvent;
    }

    UID Dataset::findSmallestAvailableNeuronUID() const
    {
        UID smallest = 0;
        while (_neurons.contains(smallest)) {
            ++smallest;
        }
        return smallest;
    }

    UID Dataset::findSmallestAvailableActivityUID() const
    {
        UID smallest = 0;
        while (_activities.contains(smallest)) {
            ++smallest;
        }
        return smallest;
    }
} // namespace mindset
