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
        rush::Vec3f position;
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
