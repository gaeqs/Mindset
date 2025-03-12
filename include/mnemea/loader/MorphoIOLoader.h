//
// Created by gaeqs on 12/03/2025.
//

#ifndef MORPHOIOLOADER_H
#define MORPHOIOLOADER_H

#include <filesystem>
#include <mnemea/Dataset.h>

namespace mnemea {
    class MorphoIOLoader {
        std::filesystem::path _path;
        UID _uid;

    public:
        MorphoIOLoader(std::filesystem::path path);

        [[nodiscard]] UID getUID() const;

        void setUID(UID uid);

        void load(Dataset& dataset) const;
    };
}

#endif //MORPHOIOLOADER_H
