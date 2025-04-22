//
// Created by gaeqs on 26/03/2025.
//

#include <catch2/catch_all.hpp>
#include <mindset/mindset.h>
#include <mindset/Contextualized.h>

TEST_CASE("Snudda load")
{
    auto cPath = getenv("SNUDDA_PATH");
    if (cPath == nullptr) {
        FAIL("SNUDDA_PATH environment variable not set.");
        return;
    }

    mindset::Dataset dataset;
    mindset::SnuddaLoader loader(cPath);
    loader.load(dataset);

    auto& props = dataset.getProperties();

    auto posProp = props.getPropertyUID(mindset::PROPERTY_POSITION);
    auto preSectionProp = props.getPropertyUID(mindset::PROPERTY_SYNAPSE_PRE_NEURITE);
    auto postSectionProp = props.getPropertyUID(mindset::PROPERTY_SYNAPSE_POST_NEURITE);

    REQUIRE(posProp.has_value());
    REQUIRE(preSectionProp.has_value());
    REQUIRE(postSectionProp.has_value());

    for (auto synapse : dataset.getCircuit().getSynapses()) {
        auto pos = synapse->getProperty<rush::Vec3f>(*posProp);
        auto pre = synapse->getProperty<mindset::UID>(*preSectionProp);
        auto post = synapse->getProperty<mindset::UID>(*postSectionProp);

        REQUIRE(pos);

        if (pre) {
            auto preNeuron = dataset.getNeuron(synapse->getPreSynapticNeuron());
            REQUIRE(preNeuron);
            REQUIRE((*preNeuron)->getMorphology());
            auto preMorphology = *(*preNeuron)->getMorphology();
            auto preNeurite = preMorphology->getNeurite(*pre);
            if (!preNeurite.has_value()) {
                // Check if the neurite is the soma.
                auto soma = preMorphology->getSoma();
                if (!soma.has_value()) {
                    FAIL(std::format("Couldn't find neurite %d", *pre));
                }
                REQUIRE(soma.value()->isRepresentedById(*pre));
            }
        }

        if (post) {
            auto postNeuron = dataset.getNeuron(synapse->getPostSynapticNeuron());
            REQUIRE(postNeuron);
            REQUIRE((*postNeuron)->getMorphology());
            auto postMorphology = *(*postNeuron)->getMorphology();
            auto postNeurite = postMorphology->getNeurite(*post);
            if (!postNeurite.has_value()) {
                // Check if the neurite is the soma.
                auto soma = postMorphology->getSoma();
                if (!soma.has_value()) {
                    FAIL(std::format("Couldn't find neurite %d", *post));
                }
                REQUIRE(soma.value()->isRepresentedById(*post));
            }
        }
    }
}

TEST_CASE("Snudda activity load")
{
    auto cPath = getenv("SNUDDA_ACTIVITY_PATH");
    if (cPath == nullptr) {
        FAIL("SNUDDA_ACTIVITY_PATH environment variable not set.");
        return;
    }

    mindset::Dataset dataset;
    mindset::SnuddaActivityLoader loader(cPath);
    loader.setLoadMorphology(false);
    loader.load(dataset);

    auto& props = dataset.getProperties();

    auto spikesProp = props.getPropertyUID(mindset::PROPERTY_ACTIVITY_SPIKES);
    auto voltageProp = props.getPropertyUID(mindset::PROPERTY_ACTIVITY_VOLTAGE);

    REQUIRE(spikesProp.has_value());
    REQUIRE(voltageProp.has_value());

    mindset::Activity* activity = *dataset.getActivities().begin();
    auto spikesOptional = activity->getPropertyPtr<mindset::EventSequence<std::monostate>>(*spikesProp);
    REQUIRE(spikesOptional.has_value());

    auto* spikes = spikesOptional.value();
    std::cout << spikes->getEvents().size() << std::endl;

    auto voltageOptional = activity->getPropertyPtr<mindset::TimeGrid<double>>(*voltageProp);
    REQUIRE(voltageOptional.has_value());

    auto* voltage = voltageOptional.value();
    std::cout << voltage->getDimensions().first << " - " << voltage->getDimensions().second << std::endl;
}