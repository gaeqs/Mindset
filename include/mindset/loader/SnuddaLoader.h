//
// Created by gaeqs on 14/03/25.
//

#ifndef SNUDDALOADER_H
#define SNUDDALOADER_H

#include <filesystem>

#include <mindset/loader/Loader.h>
#include <highfive/H5File.hpp>

namespace mindset
{
    static const std::string SNUDDA_LOADER_ID = "mindset:loader_snudda";
    static const std::string SNUDDA_LOADER_NAME = "Snudda";

    struct SnuddaLoaderProperties
    {
        UID position;

        UID neuriteRadius;
        UID neuriteParent;
        UID neuriteType;

        UID neuronTransform;

        UID synapsePreNeurite;
        UID synapsePostNeurite;
        UID synapsePrePosition;
        UID synapsePostPosition;
    };

    /**
     * This Loader loads Snudda projects.
     */
    class SnuddaLoader : public Loader
    {
        HighFive::File _file;
        std::filesystem::path _dataPath;
        bool _loadMorphology;
        bool _loadSynapses;

        SnuddaLoaderProperties initProperties(Properties& properties) const;

        void loadNeurons(Dataset& dataset,
                         const std::unordered_map<std::string, std::shared_ptr<Morphology>>& morphologies,
                         const SnuddaLoaderProperties& properties) const;

        Result<std::unordered_map<std::string, std::shared_ptr<Morphology>>, std::string> loadMorphologies(
            Dataset& dataset) const;

        std::optional<std::string> loadSynapses(Dataset& dataset, const SnuddaLoaderProperties& properties) const;

      public:
        explicit SnuddaLoader(const std::filesystem::path& path);

        [[nodiscard]] bool shouldLoadMorphology() const;

        void setLoadMorphology(bool loadMorphology);

        [[nodiscard]] bool shouldLoadSynapses() const;

        void setLoadSynapses(bool loadSynapses);

        void load(Dataset& dataset) const override;

        static LoaderFactory createFactory();
    };
} // namespace mindset

#endif // SNUDDALOADER_H
