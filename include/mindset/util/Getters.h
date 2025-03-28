//
// Created by gaeqs on 27/03/25.
//

#ifndef GETTERS_H
#define GETTERS_H

#include <mindset/Dataset.h>

namespace mindset
{

    template<typename Type>
    std::unordered_map<UID, Type> getNeuronsProperties(const Dataset& dataset, const std::string& propertyName)
    {
        auto optional = dataset.getProperties().getPropertyUID(propertyName);
        if (!optional) {
            return {};
        }
        UID property = *optional;

        std::unordered_map<UID, Type> result;
        result.reserve(dataset.getNeuronsAmount());

        for (auto neuron : dataset.getNeurons()) {
            auto value = neuron->getProperty<Type>(property);
            if (value) {
                result[neuron->getUID()] = std::move(*value);
            }
        }

        return result;
    }

    template<typename Type>
    std::vector<std::optional<Type>> getNeuronsProperties(const Dataset& dataset, const std::vector<UID>& neurons,
                                                          const std::string& propertyName)
    {
        auto optional = dataset.getProperties().getPropertyUID(propertyName);
        if (!optional) {
            return std::vector<std::optional<Type>>(neurons.size(), std::nullopt);
        }
        UID property = *optional;

        std::vector<std::optional<Type>> result;
        result.reserve(neurons.size());

        for (UID uid : neurons) {
            if (auto neuron = dataset.getNeuron(uid)) {
                result.push_back(neuron.value()->getProperty<Type>(property));
            } else {
                result.push_back(std::nullopt);
            }
        }

        return result;
    }

    template<typename Type>
    std::vector<std::optional<Type>> getNeuritesProperties(const Dataset& dataset, const Morphology& morphology,
                                                           const std::vector<UID>& neurites,
                                                           const std::string& propertyName)
    {
        auto optional = dataset.getProperties().getPropertyUID(propertyName);
        if (!optional) {
            return std::vector<std::optional<Type>>(neurites.size(), std::nullopt);
        }
        UID property = *optional;

        std::vector<std::optional<Type>> result;
        result.reserve(neurites.size());

        for (UID uid : neurites) {
            if (auto neurite = morphology.getNeurite(uid)) {
                result.push_back(neurite.value()->getProperty<Type>(property));
            } else {
                result.push_back(std::nullopt);
            }
        }

        return result;
    }

} // namespace mindset

#endif // GETTERS_H
