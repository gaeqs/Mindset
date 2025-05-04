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

#ifndef XMLLOADER_H
#define XMLLOADER_H
#include <filesystem>
#include <functional>
#include <string>
#include <optional>

#include <pugixml.hpp>

#include <mindset/Dataset.h>
#include <mindset/UID.h>
#include <mindset/util/NeuronTransform.h>
#include <mindset/loader/Loader.h>

namespace mindset
{

    static const std::string XML_LOADER_ID = "mindset:loader_xml";
    static const std::string XML_LOADER_NAME = "XML";

    /**
    * This loader loads XML Scene files.
    */
    class XMLLoader : public Loader
    {
        struct XMLNeuron
        {
            UID id;
            std::optional<UID> column;
            std::optional<UID> miniColumn;
            std::optional<UID> layer;
            std::optional<std::string> neuronType;
            std::optional<NeuronTransform> transform;
            Node* node;
        };

      public:
        using FileProvider = std::function<std::optional<std::vector<std::string>>(std::filesystem::path)>;

      private:
        pugi::xml_document _doc;
        bool _valid;

      public:
        XMLLoader(const LoaderCreateInfo& info, const void* data, size_t size);

        XMLLoader(const LoaderCreateInfo& info, std::istream& stream);

        XMLLoader(const LoaderCreateInfo& info, std::filesystem::path path);

        void load(Dataset& dataset) const override;

        static LoaderFactory createFactory();
    };
} // namespace mindset

#endif //XMLLOADER_H
