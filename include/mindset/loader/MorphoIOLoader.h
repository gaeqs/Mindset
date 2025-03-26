//
// Created by gaeqs on 12/03/2025.
//

#ifndef MORPHOIOLOADER_H
#define MORPHOIOLOADER_H
#ifdef MINDSET_BRION

    #include <filesystem>
    #include <mindset/Dataset.h>
    #include <mindset/loader/Loader.h>

namespace mindset
{

    static const std::string MORPHO_IO_LOADER_ID = "mindset:loader_morpho_io";
    static const std::string MORPHO_IO_LOADER_NAME = "Morpho IO";

    /**
    * This Loader is a wrapper for the MorphoIO file loader.
    * This loader loads .h5, .swc and .asc files.
    */
    class MorphoIOLoader : public Loader
    {
        std::filesystem::path _path;
        std::function<UID()> _provider;

      public:
        explicit MorphoIOLoader(std::filesystem::path path);

        void addUIDProvider(std::function<UID()> provider) override;

        void load(Dataset& dataset) const override;

        static LoaderFactory createFactory();
    };
} // namespace mindset

#endif
#endif //MORPHOIOLOADER_H
