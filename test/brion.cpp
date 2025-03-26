//
// Created by gaeqs on 12/03/25.
//

#include <catch2/catch_all.hpp>
#include <mindset/DefaultProperties.h>

#include <mindset/loader/BlueConfigLoader.h>
#include <mindset/util/NeuronTransform.h>
#include <rush/vector/vec.h>

TEST_CASE("Brion load")
{
    std::filesystem::path path = "/run/media/gaeqs/HDD/SynCoPaData/build/vizCa2p0_1x7/BlueConfig";
    mindset::BlueConfigLoader loader(path);
    loader.addTarget("sergio");
    loader.setLoadMorphology(true);
    loader.setLoadHierarchy(true);

    size_t i = 0;
    auto listener = loader.createListener([&](const mindset::LoaderStatus& result) {
        ++i;
        std::cout << result.currentTask << " (" << result.stagesCompleted << "/" << result.stages << ")" << std::endl;
    });

    mindset::Dataset dataset;
    loader.load(dataset);

    auto& properties = dataset.getProperties();

    for (auto& neuron : dataset.getNeurons()) {
        auto morphology = neuron.getMorphology();
        if (!morphology.has_value()) {
            continue;
        }
        auto somaOptional = morphology.value()->getSoma();
        if (!somaOptional.has_value()) {
            continue;
        }
        auto soma = somaOptional.value();
        std::cout << "Soma nodes: " << soma->getNodes().size() << std::endl;
        std::cout << "Soma pos: " << soma->getCenter() << std::endl;
        std::cout << "Soma mean radius: " << soma->getMeanRadius() << std::endl;
        std::cout << "Soma max radius: " << soma->getMaxRadius() << std::endl;
        std::cout << "Soma min radius: " << soma->getMinRadius() << std::endl;
    }
}
