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

#ifndef LOADERREGISTRY_H
#define LOADERREGISTRY_H
#include <string>
#include <unordered_map>

#include <mindset/loader/Loader.h>

namespace mindset
{
    /**
     * Registry maintaining available LoaderFactory instances, enabling easy retrieval and management of loaders.
     */
    class LoaderRegistry
    {
        std::unordered_map<std::string, LoaderFactory> _factories;

      public:
        /**
         * Constructs a LoaderRegistry and optionally loads default loaders.
         * @param loadDefaults If true, default loaders are added upon initialization.
         */
        explicit LoaderRegistry(bool loadDefaults = true);

        /**
         * Adds a LoaderFactory to the registry.
         * @param factory The loader factory to add.
         * @return True if successfully added; false if already exists.
         */
        bool add(LoaderFactory factory);

        /**
         * Removes a LoaderFactory from the registry by its ID.
         * @param id Identifier of the loader factory to remove.
         * @return True if successfully removed; false if it did not exist.
         */
        bool remove(const std::string& id);

        /**
         * Clears all loader factories from the registry.
         */
        void clear();

        /**
         * Retrieves a LoaderFactory by name, if it exists.
         * @param name Identifier of the loader factory.
         */
        std::optional<LoaderFactory> get(const std::string& name) const;

        /**
         * Provides a const range view of all registered loader factories.
         */
        [[nodiscard]] auto getAll() const
        {
            return _factories |
                   std::views::transform([](const auto& pair) -> const LoaderFactory& { return pair.second; });
        }
    };
} // namespace mindset

#endif //LOADERREGISTRY_H
