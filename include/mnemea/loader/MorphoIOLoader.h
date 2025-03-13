//
// Created by gaeqs on 12/03/2025.
//

#ifndef MORPHOIOLOADER_H
#define MORPHOIOLOADER_H

#include <filesystem>
#include <hey/Observable.h>
#include <mnemea/Dataset.h>
#include <mnemea/loader/LoaderStatus.h>

namespace mnemea {

    /**
    * This Loader is a wrapper for the MorphoIO file loader.
    * This loader loads .h5, .swc and .asc files.
    */
    class MorphoIOLoader : public hey::Observable<LoaderStatus>  {
        std::filesystem::path _path;
        UID _uid;

    public:
        explicit MorphoIOLoader(std::filesystem::path path);

        [[nodiscard]] UID getUID() const;

        void setUID(UID uid);

        void load(Dataset& dataset) const;
    };
}

#endif //MORPHOIOLOADER_H
