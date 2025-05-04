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

#ifdef MINDSET_BRION
    #include <catch2/catch_all.hpp>
    #include <mindset/Dataset.h>
    #include <mindset/DefaultProperties.h>

    #include <mindset/loader/BlueConfigLoader.h>
    #include <rush/vector/vec.h>

TEST_CASE("Brion load")
{
    auto cPath = getenv("BLUE_CONFIG_DATA");
    if (cPath == nullptr) {
        FAIL("BLUE_CONFIG_DATA environment variable not set.");
        return;
    }
    auto target = getenv("BLUE_CONFIG_TARGET");
    if (target == nullptr) {
        FAIL("BLUE_CONFIG_TARGET environment variable not set.");
        return;
    }

    std::filesystem::path path = cPath;

    auto factory = mindset::BlueConfigLoader::createFactory();

    mindset::Environment env;
    env.insert({mindset::BLUE_CONFIG_LOADER_ENTRY_TARGETS, std::vector<std::string>{target}});

    auto result = factory.create(nullptr, env, path);
    if (!result.isOk()) {
        FAIL();
    }

    auto loader = std::move(result.getResult());

    size_t i = 0;
    auto listener = loader->createListener([&](const mindset::LoaderStatus& result) {
        ++i;
        std::cout << result.currentTask << " (" << result.stagesCompleted << "/" << result.stages << ")" << std::endl;
    });

    mindset::Dataset dataset;
    loader->load(dataset);

    auto& properties = dataset.getProperties();

    for (auto neuron : dataset.getNeurons()) {
        auto morphology = neuron->getMorphology();
        if (!morphology.has_value()) {
            continue;
        }
        for (auto* neurite : morphology.value()->getNeurites()) {
        }
    }

    for (auto neuron : dataset.getNeurons()) {
        auto morphology = neuron->getMorphology();
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

    std::cout << "Loaded synapses: " << dataset.getCircuit().getSynapses().size() << std::endl;

    size_t synapseIndex = 0;
    for (auto synapse : dataset.getCircuit().getSynapses() | dataset) {
        auto pre = synapse.getProperty<mindset::UID>(mindset::PROPERTY_SYNAPSE_PRE_NEURITE);
        auto post = synapse.getProperty<mindset::UID>(mindset::PROPERTY_SYNAPSE_POST_NEURITE);

        if (!pre || !post) {
            continue;
        }

        std::cout << "Synapse UID " << synapse->getUID() << std::endl;
        std::cout << "Synapse pre " << synapse->getPreSynapticNeuron() << std::endl;
        std::cout << "Synapse pre section " << pre.value() << std::endl;
        std::cout << "Synapse post " << synapse->getPostSynapticNeuron() << std::endl;
        std::cout << "Synapse post section " << post.value() << std::endl;
        std::cout << "Synapse position " << synapse.getPosition().value_or(rush::Vec3f(0.0f)) << std::endl;
        std::cout
            << "Synapse pre pos "
            << synapse.getProperty<rush::Vec3f>(mindset::PROPERTY_SYNAPSE_PRE_POSITION).value_or(rush::Vec3f(0.0f))
            << std::endl;
        std::cout
            << "Synapse post pos "
            << synapse.getProperty<rush::Vec3f>(mindset::PROPERTY_SYNAPSE_POST_POSITION).value_or(rush::Vec3f(0.0f))
            << std::endl;

        if (synapseIndex++ > 100) {
            break;
        }
    }
}

#endif
