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

#ifndef SWCLOADER_H
#define SWCLOADER_H
#include <filesystem>
#include <string>
#include <vector>

#include <rush/rush.h>

#include <mindset/Dataset.h>
#include <mindset/UID.h>
#include <mindset/util/Result.h>
#include <mindset/loader/Loader.h>

namespace mindset
{

    static const std::string SWC_LOADER_ID = "mindset:loader_swc";
    static const std::string SWC_LOADER_NAME = "SWC";

    /**
    * This is an auxiliary SWC Loader that doesn't require Brion to work.
    */
    class SWCLoader : public Loader
    {
        struct SWCSegment
        {
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
        explicit SWCLoader(const LoaderCreateInfo& info, const std::vector<std::string>& lines);

        explicit SWCLoader(const LoaderCreateInfo& info, std::vector<std::string>&& lines);

        explicit SWCLoader(const LoaderCreateInfo& info, std::istream& stream);

        explicit SWCLoader(const LoaderCreateInfo& info, const std::filesystem::path& path);

        void addUIDProvider(std::function<UID()> provider) override;

        Result<std::shared_ptr<Morphology>, std::string> loadMorphology(Dataset& dataset) const;

        void load(Dataset& dataset) const override;

        static LoaderFactory createFactory();
    };
} // namespace mindset

#endif //SWCLOADER_H
