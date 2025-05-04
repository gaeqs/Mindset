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
    std::unordered_map<UID, Type> getNeuritesProperties(const Dataset& dataset, const Morphology& morphology,
                                                        const std::string& propertyName)
    {
        auto optional = dataset.getProperties().getPropertyUID(propertyName);
        if (!optional) {
            return {};
        }
        UID property = *optional;

        std::unordered_map<UID, Type> result;

        for (auto neurite : morphology.getNeurites()) {
            auto value = neurite->getProperty<Type>(property);
            if (value) {
                result[neurite->getUID()] = std::move(*value);
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
