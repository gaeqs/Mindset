//
// Created by gaeqs on 13/03/25.
//

#ifndef SWCLOADER_H
#define SWCLOADER_H
#include <filesystem>
#include <string>
#include <vector>

#include <rush/rush.h>

#include <mnemea/Dataset.h>
#include <mnemea/UID.h>
#include <mnemea/util/Result.h>
#include <mnemea/loader/Loader.h>


namespace mnemea {


    static const std::string SWC_LOADER_ID = "mnemea:loader_swc";
    static const std::string SWC_LOADER_NAME = "SWC";

    /**
    * This is an auxiliary SWC Loader that doesn't require Brion to work.
    */
    class SWCLoader : public Loader {
        struct SWCSegment {
            UID id;
            uint32_t type;
            rush::Vec3f end;
            float radius;
            int64_t parent;
        };

        std::function<UID()> _provider;
        std::vector<std::string> _lines;

        [[nodiscard]] Result<SWCSegment, std::string> toSegment(size_t lineIndex) const;

    public:
        explicit SWCLoader(const std::vector<std::string>& lines);

        explicit SWCLoader(std::vector<std::string>&& lines);

        explicit SWCLoader(std::istream& stream);

        explicit SWCLoader(const std::filesystem::path& path);

        void addUIDProvider(std::function<UID()> provider) override;

        Result<std::shared_ptr<Morphology>, std::string> loadMorphology(Dataset& dataset) const;

        void load(Dataset& dataset) const override;

        static LoaderFactory createFactory();
    };
}

#endif //SWCLOADER_H
