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

#ifndef SNUDDALOADER_H
#define SNUDDALOADER_H

#include <filesystem>

#include <mindset/loader/Loader.h>
#include <highfive/H5File.hpp>

namespace mindset
{
    static const std::string SNUDDA_LOADER_ID = "mindset:loader_snudda";
    static const std::string SNUDDA_LOADER_NAME = "Snudda";
    static const std::string SNUDDA_LOADER_ENTRY_SNUDDA_DATA_PATH = "mindset:snudda_data_path";
    static const std::string SNUDDA_LOADER_ENTRY_LOAD_MORPHOLOGY = "mindset:load_morphology";
    static const std::string SNUDDA_LOADER_ENTRY_LOAD_SYNAPSES = "mindset:load_synapses";
    static const std::string SNUDDA_LOADER_ENTRY_LOAD_ACTIVITY = "mindset:load_activity";

    static constexpr std::array SNUDDA_LOADER_VALID_ID_GROUPS = {
        "network/neurons/neuron_id",
        "meta_data/id",
    };

    static constexpr std::array SNUDDA_LOADER_VALID_POSITION_GROUPS = {
        "network/neurons/position",
        "meta_data/position",
    };

    static constexpr std::array SNUDDA_LOADER_VALID_ROTATION_GROUPS = {
        "network/neurons/rotation",
    };

    static constexpr std::array SNUDDA_LOADER_VALID_MORPHOLOGY_GROUPS = {
        "network/neurons/morphology",
        "meta_data/morphology",
    };

    static constexpr std::array SNUDDA_LOADER_VALID_VOXEL_SIZE_GROUPS = {
        "meta/voxel_size"
    };
    static constexpr std::array SNUDDA_LOADER_VALID_SYNAPSES_GROUPS = {
        "network/synapses"
    };
    static constexpr std::array SNUDDA_LOADER_VALID_SIMULATION_ORIGO_GROUPS = {
        "meta/simulation_origo"
    };

    struct SnuddaLoaderProperties
    {
        std::string snuddaPath;
        std::vector<uint64_t> ids;

        bool loadMorphologies;
        bool loadSynapses;
        bool loadActivity;

        UID position;

        UID morphologyPath;
        UID neuriteRadius;
        UID neuriteParent;
        UID neuriteType;

        UID neuronTransform;

        UID synapsePreNeurite;
        UID synapsePostNeurite;
        UID synapsePrePosition;
        UID synapsePostPosition;

        UID activitySpikes;
        UID activityVoltage;

        std::optional<std::string> positionGroup;
        std::optional<std::string> rotationGroup;
        std::optional<std::string> morphologyGroup;

        std::optional<std::string> voxelSizeGroup;
        std::optional<std::string> synapsesGroup;
        std::optional<std::string> simulationOrigoGroup;
    };

    /**
     * This Loader loads Snudda projects.
     */
    class SnuddaLoader : public Loader
    {
        HighFive::File _file;

        SnuddaLoaderProperties initProperties(Dataset& dataset, const std::string& snuddaPath,
                                              std::vector<uint64_t> ids) const;

        void loadNeurons(Dataset& dataset,
                         const std::unordered_map<std::string, std::shared_ptr<Morphology>>& morphologies,
                         const SnuddaLoaderProperties& properties) const;

        Result<std::unordered_map<std::string, std::shared_ptr<Morphology>>, std::string> loadMorphologies(
            const SnuddaLoaderProperties& properties, Dataset& dataset) const;

        void loadSynapses(Dataset& dataset, const SnuddaLoaderProperties& properties) const;

        void loadOutputActivity(Dataset& dataset, const SnuddaLoaderProperties& properties) const;

        void loadInputActivity(Dataset& dataset, const SnuddaLoaderProperties& properties) const;

      public:
        explicit SnuddaLoader(const LoaderCreateInfo& info, const std::filesystem::path& path);

        void load(Dataset& dataset) const override;

        static LoaderFactory createFactory();
    };
} // namespace mindset

#endif // SNUDDALOADER_H
