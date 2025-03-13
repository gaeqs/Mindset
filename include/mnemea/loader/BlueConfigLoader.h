//
// Created by gaeqs on 12/03/2025.
//

#ifndef BLUECONFIGLOADER_H
#define BLUECONFIGLOADER_H

#include <filesystem>
#include <brain/python/types.h>
#include <brion/brion.h>
#include <mnemea/Dataset.h>

namespace mnemea {
    struct BlueConfigLoaderProperties {
        UID neuritePosition;
        UID neuriteRadius;
        UID neuriteParent;
        UID neuriteType;

        UID neuronTransform;
        UID neuronColumn;
        UID neuronMiniColumn;
        UID neuronLayer;
    };

    /**
    * This Loader loads BlueConfig files.
    */
    class BlueConfigLoader {
        brion::BlueConfig _blueConfig;
        std::set<std::string> _targets;
        bool _loadMorphology;
        bool _loadHierarchy;

        BlueConfigLoaderProperties initProperties(Properties& properties) const;

        static void loadNeurons(Dataset& dataset,
                                const BlueConfigLoaderProperties& properties,
                                const brion::GIDSet& ids,
                                const brain::Circuit& circuit);

        static void loadMorphologies(Dataset& dataset,
                                     const BlueConfigLoaderProperties& properties,
                                     const brion::GIDSet& ids,
                                     const brain::Circuit& circuit);

        static void loadHierarchy(Dataset& dataset,
                                  const BlueConfigLoaderProperties& properties,
                                  const brion::GIDSet& ids,
                                  const brion::Circuit& circuit);

        static std::shared_ptr<Morphology> loadMorphology(const BlueConfigLoaderProperties& properties,
                                                          const brion::Morphology& morphology);

    public:
        BlueConfigLoader(std::filesystem::path path);

        bool addTarget(std::string target);

        [[nodiscard]] std::set<std::string>& getTargets();

        [[nodiscard]] const std::set<std::string>& getTargets() const;

        [[nodiscard]] bool shouldLoadMorphology() const;

        void setLoadMorphology(bool loadMorphology);

        [[nodiscard]] bool shouldLoadHierarchy() const;

        void setLoadHierarchy(bool loadHierarchy);

        void load(Dataset& dataset) const;
    };
}

#endif //BLUECONFIGLOADER_H
