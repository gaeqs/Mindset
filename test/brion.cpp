//
// Created by gaeqs on 12/03/25.
//

#include <catch2/catch_all.hpp>
#include <mnemea/DefaultProperties.h>

#include <mnemea/loader/BlueConfigLoader.h>
#include <mnemea/util/NeuronTransform.h>
#include <rush/vector/vec.h>

TEST_CASE("Brion load") {
    std::filesystem::path path = "/run/media/gaeqs/HDD/SynCoPaData/build/vizCa2p0_1x7/BlueConfig";
    mnemea::BlueConfigLoader loader(path);
    loader.setLoadMorphology(false);
    loader.setLoadHierarchy(true);

    mnemea::Dataset dataset;
    loader.load(dataset);

    auto& properties = dataset.getProperties();

    mnemea::UID position = properties.getPropertyUID(mnemea::PROPERTY_TRANSFORM).value();
    mnemea::UID column = properties.getPropertyUID(mnemea::PROPERTY_COLUMN).value();
    mnemea::UID miniColumn = properties.getPropertyUID(mnemea::PROPERTY_MINI_COLUMN).value();
    for (auto& neuron: dataset.getNeurons()) {
        std::cout << neuron.getProperty<mnemea::NeuronTransform>(position).value().getPosition() << std::endl;
        std::cout << neuron.getProperty<mnemea::UID>(column).value() << std::endl;
        std::cout << neuron.getProperty<mnemea::UID>(miniColumn).value() << std::endl;
        std::cout << neuron.getUID() << std::endl;
    }
}
