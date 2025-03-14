//
// Created by gaeqs on 14/03/25.
//

#ifndef SNUDDALOADER_H
#define SNUDDALOADER_H

#include <filesystem>

#include <mnemea/loader/Loader.h>
#include <deps/libsonata/extlib/HighFive/include/highfive/H5File.hpp>


namespace mnemea {

    static const std::string SNUDDA_LOADER_ID = "mnemea:loader_snudda";
    static const std::string SNUDDA_LOADER_NAME = "Snudda";


    class SnuddaLoader : public Loader {

        HighFive::File _file;

    public:
        explicit SnuddaLoader(const std::filesystem::path& path);

        void load(Dataset& dataset) const override;

        static LoaderFactory createFactory();

    };
}

#endif //SNUDDALOADER_H
