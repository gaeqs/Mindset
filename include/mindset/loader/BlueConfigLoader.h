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

    static const std::string BLUE_CONFIG_LOADER_ENTRY_TARGETS = "mindset:targets";
    static const std::string BLUE_CONFIG_LOADER_ENTRY_LOAD_MORPHOLOGY = "mindset:load_morphology";
    static const std::string BLUE_CONFIG_LOADER_ENTRY_LOAD_HIERARCHY = "mindset:load_hierarchy";
    static const std::string BLUE_CONFIG_LOADER_ENTRY_LOAD_SYNAPSES = "mindset:load_synapses";

    /**
     * This Loader loads BlueConfig files.
     */
    class BlueConfigLoader : public Loader
    {
        brion::BlueConfig _blueConfig;

        BlueConfigLoaderProperties initProperties(Dataset& dataset, bool shouldLoadMorphologies,
                                                  bool shouldLoadSynapses, bool shouldLoadHierarchy) const;

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
        explicit BlueConfigLoader(const LoaderCreateInfo& info, std::filesystem::path path);

        void load(Dataset& dataset) const override;

        static LoaderFactory createFactory();
    };
} // namespace mindset

#endif
#endif // BLUECONFIGLOADER_H
