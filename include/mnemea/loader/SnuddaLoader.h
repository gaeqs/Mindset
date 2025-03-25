//
// Created by gaeqs on 14/03/25.
//

#ifndef SNUDDALOADER_H
#define SNUDDALOADER_H

#include <filesystem>

#include <mnemea/loader/Loader.h>
#include <highfive/H5File.hpp>


namespace mnemea {
    static const std::string SNUDDA_LOADER_ID = "mnemea:loader_snudda";
    static const std::string SNUDDA_LOADER_NAME = "Snudda";

    struct SnuddaLoaderProperties {
        UID neuritePosition;
        UID neuriteRadius;
        UID neuriteParent;
        UID neuriteType;

        UID neuronTransform;
    };


    /**
    * This Loader loads Snudda projects.
    */
    class SnuddaLoader : public Loader {
        HighFive::File _file;
        std::filesystem::path _dataPath;
        bool _loadMorphology;
        bool _loadSynapses;

        SnuddaLoaderProperties initProperties(Properties& properties) const;

        void loadNeurons(Dataset& dataset, const SnuddaLoaderProperties& properties) const;

        std::optional<std::string> loadMorphologies(Dataset& dataset) const;

        std::optional<std::string> loadSynapses(Dataset& dataset, const SnuddaLoaderProperties& properties) const;

        static void assignMorphology(Dataset& dataset, UID uid, std::shared_ptr<Morphology> morphology);

    public:
        explicit SnuddaLoader(const std::filesystem::path& path);

        [[nodiscard]] bool shouldLoadMorphology() const;

        void setLoadMorphology(bool loadMorphology);

        [[nodiscard]] bool shouldLoadSynapses() const;

        void setLoadSynapses(bool loadSynapses);

        void load(Dataset& dataset) const override;

        static LoaderFactory createFactory();
    };
}

#endif //SNUDDALOADER_H
