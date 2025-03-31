//
// Created by gaeqs on 12/03/25.
//

#ifdef MINDSET_BRION
    #include <catch2/catch_all.hpp>
    #include <mindset/Dataset.h>
    #include <mindset/DefaultProperties.h>

    #include <mindset/loader/BlueConfigLoader.h>
    #include <mindset/util/NeuronTransform.h>
    #include <rush/vector/vec.h>

TEST_CASE("Brion load")
{
    std::filesystem::path path = "/run/media/gaeqs/HDD/SynCoPaData/build/vizCa2p0_1x7/BlueConfig";
    mindset::BlueConfigLoader loader(path);
    loader.addTarget("felix");
    loader.setLoadMorphology(true);
    loader.setLoadHierarchy(true);
    loader.setLoadSynapses(true);

    size_t i = 0;
    auto listener = loader.createListener([&](const mindset::LoaderStatus& result) {
        ++i;
        std::cout << result.currentTask << " (" << result.stagesCompleted << "/" << result.stages << ")" << std::endl;
    });

    mindset::Dataset dataset;
    loader.load(dataset);

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

        auto pre = synapse.getProperty<mindset::UID>(mindset::PROPERTY_SYNAPSE_PRE_SECTION);
        auto post = synapse.getProperty<mindset::UID>(mindset::PROPERTY_SYNAPSE_POST_SECTION);

        if (!pre || !post) continue;

        std::cout << "Synapse UID " << synapse->getUID() << std::endl;
        std::cout << "Synapse pre " << synapse->getPreSynapticNeuron() << std::endl;
        std::cout << "Synapse pre section " << pre.value() << std::endl;
        std::cout << "Synapse post " << synapse->getPostSynapticNeuron() << std::endl;
        std::cout << "Synapse post section " << post.value() << std::endl;
        std::cout << "Synapse position " << synapse.getPosition().value_or(rush::Vec3f(0.0f)) << std::endl;
        std::cout << "Synapse pre pos " << synapse.getProperty<rush::Vec3f>(mindset::PROPERTY_SYNAPSE_PRE_POSITION).value_or(rush::Vec3f(0.0f)) << std::endl;
        std::cout << "Synapse post pos " << synapse.getProperty<rush::Vec3f>(mindset::PROPERTY_SYNAPSE_POST_POSITION).value_or(rush::Vec3f(0.0f)) << std::endl;

        if (synapseIndex++ > 100) break;
    }
}

#endif
