//
// Created by gaeqs on 12/03/2025.
//

#ifndef BLUECONFIGLOADER_H
#define BLUECONFIGLOADER_H
#ifdef MINDSET_BRION

    #include <filesystem>
    #include <brion/brion.h>
    #include <brain/brain.h>

    #include <mindset/Dataset.h>
    #include <mindset/loader/Loader.h>

namespace mindset
{
    struct BlueConfigLoaderProperties
    {
        UID position; // "mindset:position"

        UID neuriteParent; // "mindset:parent"
        UID neuriteRadius; // "mindset:radius"
        UID neuriteType;   // "mindset:neurite_type"

        UID neuronTransform;  // "mindset:transform"
        UID neuronColumn;     // "mindset:column"
        UID neuronMiniColumn; // "mindset:mini_column"
        UID neuronLayer;      // "mindset:layer"
        UID neuronName;       // "mindset:name"

        UID synapsePreNeurite;   // "mindset:synapse_pre_neurite"
        UID synapsePostNeurite;  // "mindset:synapse_post_neurite"
        UID synapsePrePosition;  // "mindset:synapse_pre_position"
        UID synapsePostPosition; // "mindset:synapse_post_position"
        UID synapseDelay;        // "mindset:synapse_delay"
        UID synapseConductance;  // "mindset:synapse_conductance"
        UID synapseUtilization;  // "mindset:synapse_utilization"
        UID synapseDepression;   // "mindset:synapse_depression"
        UID synapseFacilitation; // "mindset:synapse_facilitation"
        UID synapseDecay;        // "mindset:synapse_decay"
        UID synapseEfficacy;     // "mindset:synapse_efficacy"
    };

    static const std::string BLUE_CONFIG_LOADER_ID = "mindset:loader_blue_config";
    static const std::string BLUE_CONFIG_LOADER_NAME = "Blue Config";

    /**
     * This Loader loads BlueConfig files.
     */
    class BlueConfigLoader : public Loader
    {
        brion::BlueConfig _blueConfig;
        std::set<std::string> _targets;
        bool _loadMorphology;
        bool _loadHierarchy;
        bool _loadSynapses;

        BlueConfigLoaderProperties initProperties(Properties& properties) const;

        static void loadNeurons(Dataset& dataset, const BlueConfigLoaderProperties& properties,
                                const brion::GIDSet& ids, const brain::Circuit& circuit,
                                const std::map<std::string, std::shared_ptr<Morphology>>& morphologies);

        static std::map<std::string, std::shared_ptr<Morphology>> loadMorphologies(
            const BlueConfigLoaderProperties& properties, const brion::GIDSet& ids, const brain::Circuit& circuit);

        static void loadSynapses(Dataset& dataset, const BlueConfigLoaderProperties& properties,
                                 const brion::GIDSet& ids, const brain::Circuit& circuit);

        static void loadHierarchy(Dataset& dataset, const BlueConfigLoaderProperties& properties,
                                  const brion::GIDSet& ids, const brion::Circuit& circuit);

        static std::shared_ptr<Morphology> loadMorphology(const BlueConfigLoaderProperties& properties,
                                                          const brion::Morphology& morphology);

      public:
        explicit BlueConfigLoader(std::filesystem::path path);

        bool addTarget(std::string target);

        [[nodiscard]] std::set<std::string>& getTargets();

        [[nodiscard]] const std::set<std::string>& getTargets() const;

        [[nodiscard]] bool shouldLoadMorphology() const;

        void setLoadMorphology(bool loadMorphology);

        [[nodiscard]] bool shouldLoadHierarchy() const;

        void setLoadHierarchy(bool loadHierarchy);

        [[nodiscard]] bool shouldLoadSynapses() const;

        void setLoadSynapses(bool loadSynapses);

        void load(Dataset& dataset) const override;

        static LoaderFactory createFactory();
    };
} // namespace mindset

#endif
#endif // BLUECONFIGLOADER_H
