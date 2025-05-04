// Copyright (c) 2025. VG-Lab/URJC.
//
// Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
//
// This file is part of Mindset <https://gitlab.gmrv.es/g.rial/mindset>
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 3.0 as published
// by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

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
        explicit MorphoIOLoader(const LoaderCreateInfo& info, std::filesystem::path path);

        void addUIDProvider(std::function<UID()> provider) override;

        void load(Dataset& dataset) const override;

        static LoaderFactory createFactory();
    };
} // namespace mindset

#endif
#endif //MORPHOIOLOADER_H
