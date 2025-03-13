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

namespace mnemea {
    /**
    * This is an auxiliary SWC Loader that doesn't require Brion to work.
    */
    class SWCLoader {
        struct SWCSegment {
            UID id;
            uint32_t type;
            rush::Vec3f end;
            float radius;
            int64_t parent;
        };

        UID _uid;
        std::vector<std::string> _lines;

        [[nodiscard]] Result<SWCSegment, std::string> toSegment(size_t lineIndex) const;

    public:
        explicit SWCLoader(const std::vector<std::string>& lines);

        explicit SWCLoader(std::vector<std::string>&& lines);

        explicit SWCLoader(std::istream& stream);

        explicit SWCLoader(const std::filesystem::path& path);

        [[nodiscard]] UID getUID() const;

        void setUID(UID uid);

        void load(Dataset& dataset) const;
    };
}

#endif //SWCLOADER_H
