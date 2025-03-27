//
// Created by gaeqs on 26/03/2025.
//

#include <catch2/catch_all.hpp>
#include <mindset/mindset.h>

TEST_CASE("Test")
{
    mindset::Dataset dataset;
    mindset::SWCLoader loader(std::filesystem::current_path() / "data/test.swc");
    loader.load(dataset);

    REQUIRE(dataset.getNeurons().size() == 1);

    for (auto& neuron : dataset.getNeurons()) {
        REQUIRE(neuron.getMorphology().has_value());
        auto* morphology = neuron.getMorphology().value();
        auto* tree = morphology->getOrCreateMorphologyTree(dataset);
        REQUIRE(tree->getRoot().has_value());
        std::cout << tree->getRoot().value()->getUID() << std::endl;
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
        if (!section.has_value()) continue; // No section!
        auto position = section.value()->getProperty<rush::Vec3f>(posProperty);
        if (!position.has_value()) continue; // No position!
        positions.push_back(position.value());
    }

    for (mindset::UID sectionId : postToSoma) {
        auto section = preTree->getSection(sectionId);
        if (!section.has_value()) continue; // No section!
        auto position = section.value()->getProperty<rush::Vec3f>(posProperty);
        if (!position.has_value()) continue; // No position!
        positions.push_back(position.value());
    }

    return positions;
}
