//
// Created by gaeqs on 28/03/25.
//

#ifndef MORPHOLOGYUTILS_H
#define MORPHOLOGYUTILS_H

#include <rush/rush.h>
#include <mindset/MorphologyTree.h>
#include <mindset/util/NeuronTransform.h>

namespace mindset
{

    struct ClosestNeuriteResult
    {
        bool valid;
        UID uid;
        float distanceSquared;
        float t; // Position = parentPosition + (position - parentPosition) * t
    };

    std::vector<ClosestNeuriteResult> closestNeuriteToPosition(const Dataset& dataset, const Morphology& morphology,
                                                               const std::vector<rush::Vec3f>& points,
                                                               const NeuronTransform* transform = nullptr);

    ClosestNeuriteResult closestNeuriteToPosition(const Dataset& dataset, const Morphology& morphology,
                                                  const rush::Vec3f& point, const NeuronTransform* transform = nullptr);

    std::optional<Morphology*> getMorphology(Dataset& dataset, UID neuronId);

    std::optional<const Morphology*> getMorphology(const Dataset& dataset, UID neuronId);

    std::optional<MorphologyTree*> getMorphologyTree(Dataset& dataset, UID neuronId);

    std::optional<const MorphologyTree*> getMorphologyTree(const Dataset& dataset, UID neuronId);

    std::optional<MorphologyTreeSection*> getMorphologyTreeSection(Dataset& dataset, UID neuronId, UID sectionId);

    std::optional<const MorphologyTreeSection*> getMorphologyTreeSection(const Dataset& dataset, UID neuronId,
                                                                         UID sectionId);

    std::optional<MorphologyTreeSection*> getMorphologyTreeSection(Morphology* morphology, UID sectionId);

    std::optional<const MorphologyTreeSection*> getMorphologyTreeSection(const Morphology* morphology, UID sectionId);
} // namespace mindset

#endif // MORPHOLOGYUTILS_H
