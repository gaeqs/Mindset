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

TEST_CASE("Snudda load")
{
    auto cPath = getenv("SNUDDA_PATH");
    if (cPath == nullptr) {
        FAIL("SNUDDA_PATH environment variable not set.");
        return;
    }

    auto factory = mindset::SnuddaLoader::createFactory();
    auto result = factory.create(nullptr, {}, cPath);
    REQUIRE(result.isOk());

    auto loader = std::move(result.getResult());

    mindset::Dataset dataset;
    loader->load(dataset);

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

    auto factory = mindset::SnuddaLoader::createFactory();
    auto result = factory.create(nullptr, {}, cPath);
    REQUIRE(result.isOk());

    auto loader = std::move(result.getResult());

    hey::Listener<mindset::LoaderStatus> listener = [](const mindset::LoaderStatus& status) {
        std::cout << status.currentTask << " (" << status.stagesCompleted << "/" << status.stages << ")" << std::endl;
    };
    loader->addListener(listener);

    mindset::Dataset dataset;
    loader->load(dataset);
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