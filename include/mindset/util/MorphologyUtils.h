//
// Created by gaeqs on 28/03/25.
//

#ifndef MORPHOLOGYUTILS_H
#define MORPHOLOGYUTILS_H

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

} // namespace mindset

#endif // MORPHOLOGYUTILS_H
