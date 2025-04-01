//
// Created by gaeqs on 28/03/25.
//

#include <ranges>
#include <optional>
#include <limits>

#include <mindset/Dataset.h>
#include <mindset/Morphology.h>
#include <mindset/UID.h>
#include <mindset/util/Getters.h>
#include <mindset/util/MorphologyUtils.h>

namespace
{
    std::pair<float, float> squaredDistanceToLine(const rush::Vec3f& from, const rush::Vec3f& to,
                                                  const rush::Vec3f& point)
    {
        auto ab = to - from;
        auto ap = point - from;

        float t = std::clamp(ap.dot(ab) / ab.squaredLength(), 0.0f, 1.0f);
        return std::make_pair((from + ab * t - point).squaredLength(), t);
    }
} // namespace

namespace mindset
{

    ClosestNeuriteResult closestNeuriteToPosition(const Dataset& dataset, const Morphology& morphology,
                                                  const rush::Vec3f& point, const NeuronTransform* transform)
    {
        // This is faster because there's only one point!
        rush::Vec3f localPoint = point;
        if (transform != nullptr) {
            localPoint = transform->positionToLocalCoordinates(point);
        }

        auto positions = getNeuritesProperties<rush::Vec3f>(dataset, morphology, PROPERTY_POSITION);
        auto parents = getNeuritesProperties<UID>(dataset, morphology, PROPERTY_PARENT);

        std::optional<UID> result;
        float resultT = 0;
        rush::Vec3f resultPosition;
        float resultDistance = std::numeric_limits<float>::max();

        for (UID uid : morphology.getNeuritesUIDs()) {
            auto position = positions.find(uid);
            auto parent = parents.find(uid);
            if (position == positions.end() || parent == parents.end()) {
                continue;
            }
            auto parentPosition = positions.find(parent->second);
            if (parentPosition == positions.end()) {
                continue;
            }

            auto from = parentPosition->second;
            auto to = position->second;
            auto [distance, t] = squaredDistanceToLine(from, to, localPoint);
            if (distance < resultDistance) {
                resultDistance = distance;
                resultT = t;
                result = uid;
                resultPosition = from + (to - from) * t;
            }
        }

        if (transform != nullptr) {
            resultPosition = transform->positionToGlobalCoordinates(resultPosition);
        }

        return {result.has_value(), result.value_or(0), resultDistance, resultT, resultPosition};
    }

    std::optional<Morphology*> getMorphology(Dataset& dataset, UID neuronId)
    {
        auto neuronOptional = dataset.getNeuron(neuronId);
        if (!neuronOptional) {
            return {};
        }
        return neuronOptional.value()->getMorphology();
    }

    std::optional<const Morphology*> getMorphology(const Dataset& dataset, UID neuronId)
    {
        auto neuronOptional = dataset.getNeuron(neuronId);
        if (!neuronOptional) {
            return {};
        }
        return neuronOptional.value()->getMorphology();
    }

    std::optional<MorphologyTree*> getMorphologyTree(Dataset& dataset, UID neuronId)
    {
        auto neuronOptional = dataset.getNeuron(neuronId);
        if (!neuronOptional) {
            return {};
        }
        auto morphologyOptional = neuronOptional.value()->getMorphology();
        if (!morphologyOptional) {
            return {};
        }

        return morphologyOptional.value()->getMorphologyTree();
    }

    std::optional<const MorphologyTree*> getMorphologyTree(const Dataset& dataset, UID neuronId)
    {
        auto neuronOptional = dataset.getNeuron(neuronId);
        if (!neuronOptional) {
            return {};
        }
        auto morphologyOptional = neuronOptional.value()->getMorphology();
        if (!morphologyOptional) {
            return {};
        }

        return morphologyOptional.value()->getMorphologyTree();
    }

    std::optional<MorphologyTreeSection*> getMorphologyTreeSection(Dataset& dataset, UID neuronId, UID sectionId)
    {
        auto tree = getMorphologyTree(dataset, neuronId);
        if (!tree) {
            return {};
        }

        return tree.value()->getSection(sectionId);
    }

    std::optional<const MorphologyTreeSection*> getMorphologyTreeSection(const Dataset& dataset, UID neuronId,
                                                                         UID sectionId)
    {
        auto tree = getMorphologyTree(dataset, neuronId);
        if (!tree) {
            return {};
        }

        return tree.value()->getSection(sectionId);
    }

    std::optional<MorphologyTreeSection*> getMorphologyTreeSection(Morphology* morphology, UID sectionId)
    {
        auto tree = morphology->getMorphologyTree();
        if (!tree) {
            return {};
        }

        return tree.value()->getSection(sectionId);
    }

    std::optional<const MorphologyTreeSection*> getMorphologyTreeSection(const Morphology* morphology, UID sectionId)
    {
        auto tree = morphology->getMorphologyTree();
        if (!tree) {
            return {};
        }

        return tree.value()->getSection(sectionId);
    }

    std::vector<ClosestNeuriteResult> closestNeuriteToPosition(const Dataset& dataset, const Morphology& morphology,
                                                               const std::vector<rush::Vec3f>& points,
                                                               const NeuronTransform* transform)
    {
        auto positions = getNeuritesProperties<rush::Vec3f>(dataset, morphology, PROPERTY_POSITION);
        auto parents = getNeuritesProperties<UID>(dataset, morphology, PROPERTY_PARENT);

        // This is faster because there's only one point!
        std::vector<rush::Vec3f> localPoints = points;
        if (transform != nullptr) {
            for (auto& point : localPoints) {
                point = transform->positionToLocalCoordinates(point);
            }
        }

        std::vector<ClosestNeuriteResult> results(localPoints.size(), {.valid = false});

        for (UID uid : morphology.getNeuritesUIDs()) {
            auto position = positions.find(uid);
            auto parent = parents.find(uid);
            if (position == positions.end() || parent == parents.end()) {
                continue;
            }
            auto parentPosition = positions.find(parent->second);
            if (parentPosition == positions.end()) {
                continue;
            }

            for (size_t i = 0; i < localPoints.size(); i++) {
                auto& result = results[i];
                auto from = parentPosition->second;
                auto to = position->second;
                auto [distance, t] = squaredDistanceToLine(parentPosition->second, position->second, localPoints[i]);
                if (!result.valid || distance < result.distanceSquared) {
                    result.distanceSquared = distance;
                    result.t = t;
                    result.uid = uid;
                    result.valid = true;
                    result.position = from + (to - from) * t;
                }
            }
        }

        if (transform != nullptr) {
            for (auto& result : results) {
                result.position = transform->positionToGlobalCoordinates(result.position);
            }
        }

        return results;
    }

} // namespace mindset