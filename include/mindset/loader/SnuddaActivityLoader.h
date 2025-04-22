//
// Created by gaeqs on 14/03/25.
//

#ifndef SNUDDAACTIVITYLOADER_H
#define SNUDDAACTIVITYLOADER_H

#include <filesystem>

#include <mindset/loader/Loader.h>
#include <highfive/H5File.hpp>

namespace mindset
{
    static const std::string SNUDDA_ACTIVITY_LOADER_ID = "mindset:loader_snudda_activity";
    static const std::string SNUDDA_ACTIVITY_LOADER_NAME = "Snudda (Activity)";

    struct SnuddaActivityLoaderProperties
    {
        UID position;

        UID neuriteRadius;
        UID neuriteParent;
        UID neuriteType;

        UID neuronTransform;

        UID activitySpikes;
        UID activityVoltage;
    };

    /**
     * This Loader loads Snudda projects.
     */
    class SnuddaActivityLoader : public Loader
    {
        HighFive::File _file;
        std::filesystem::path _dataPath;
        bool _loadMorphology;

        SnuddaActivityLoaderProperties initProperties(Properties& properties) const;

        void loadNeurons(Dataset& dataset,
                         const std::unordered_map<std::string, std::shared_ptr<Morphology>>& morphologies,
                         const SnuddaActivityLoaderProperties& properties) const;

        Result<std::unordered_map<std::string, std::shared_ptr<Morphology>>, std::string> loadMorphologies(
            Dataset& dataset) const;

        void loadActivity(Dataset& dataset, const SnuddaActivityLoaderProperties& properties) const;

      public:
        explicit SnuddaActivityLoader(const std::filesystem::path& path);

        [[nodiscard]] bool shouldLoadMorphology() const;

        void setLoadMorphology(bool loadMorphology);

        void load(Dataset& dataset) const override;

        static LoaderFactory createFactory();
    };
} // namespace mindset

#endif // SNUDDAACTIVITYLOADER_H
