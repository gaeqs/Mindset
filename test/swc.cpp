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

#include <catch2/catch_all.hpp>
#include <mindset/mindset.h>
#include <mindset/Contextualized.h>

TEST_CASE("Test")
{
    mindset::Dataset dataset;
    mindset::SWCLoader loader(mindset::LoaderCreateInfo(), std::filesystem::current_path() / "data/test.swc");
    loader.load(dataset);

    REQUIRE(dataset.getNeurons().size() == 1);

    for (const auto neuron : dataset.getNeurons()) {
        // Neurons are already contextualized.
        for (auto neurite : neuron->getMorphology().value()->getNeurites() | dataset) {
            // Pipe "|" operator contextualizes the neurites.
            if (auto pos = neurite.getPosition()) {
                std::cout << *pos << std::endl;
            } else {
                std::cout << " - " << std::endl;
            }
        }
    }
}

std::vector<rush::Vec3f> walkSynapse(mindset::Dataset& dataset, mindset::Neuron& pre, mindset::Neuron& post,
                                     mindset::UID preNeurite, mindset::UID postNeurite)
{
    auto posOptional = dataset.getProperties().getPropertyUID(mindset::PROPERTY_POSITION);
    if (!posOptional.has_value()) {
        return std::vector<rush::Vec3f>();
    }

    mindset::UID posProperty = posOptional.value();

    // We suppose both neurons have morphologies.
    auto* preTree = pre.getMorphology().value()->getOrCreateMorphologyTree(dataset);
    auto* postTree = pre.getMorphology().value()->getOrCreateMorphologyTree(dataset);

    auto preToSoma = preTree->flatWalkToRoot(preNeurite);
    auto postToSoma = postTree->flatWalkToRoot(postNeurite);

    std::vector<rush::Vec3f> positions;
    for (mindset::UID sectionId : preToSoma | std::views::reverse) {
        auto section = preTree->getSection(sectionId);
        if (!section.has_value()) {
            continue; // No section!
        }
        auto position = section.value()->getProperty<rush::Vec3f>(posProperty);
        if (!position.has_value()) {
            continue; // No position!
        }
        positions.push_back(position.value());
    }

    for (mindset::UID sectionId : postToSoma) {
        auto section = preTree->getSection(sectionId);
        if (!section.has_value()) {
            continue; // No section!
        }
        auto position = section.value()->getProperty<rush::Vec3f>(posProperty);
        if (!position.has_value()) {
            continue; // No position!
        }
        positions.push_back(position.value());
    }

    return positions;
}
