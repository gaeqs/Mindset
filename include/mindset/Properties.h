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

#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <map>
#include <string>
#include <optional>

#include <mindset/UID.h>

namespace mindset
{
    /**
     * Manages a set of named properties with associated unique identifiers.
     */
    class Properties
    {
        std::map<std::string, UID> _properties;
        std::map<UID, std::string> _propertiesNames;

      public:
        /**
         * Creates a new Properties instance.
         */
        Properties();

        /**
         * Defines a new property by name, auto-generating a unique UID.
         * @param name Name of the property.
         * @return UID assigned to the property.
         */
        UID defineProperty(std::string name);

        /**
         * Defines a new property by explicitly specifying its UID.
         * @param name Name of the property.
         * @param id Explicit UID for the property.
         */
        void defineProperty(std::string name, UID id);

        /**
         * Checks whether a property is defined.
         * @param name Name of the property.
         */
        [[nodiscard]] bool isPropertyDefined(const std::string& name) const;

        /**
         * Removes the property with the given name.
         * @return Whether a property was erased.
         */
        bool removeProperty(const std::string& name);

        /**
         * Retrieves the UID of a property by name, if defined.
         * @param name Name of the property.
         */
        [[nodiscard]] std::optional<UID> getPropertyUID(const std::string& name) const;

        /**
         * Retrieves the name of a property by UID, if defined.
         * @param uid UID of the property.
         */
        [[nodiscard]] std::optional<std::string> getPropertyName(UID uid) const;

        /**
         * Returns a map of property names to their UIDs.
         */
        [[nodiscard]] const std::map<std::string, UID>& getPropertiesUIDs() const;

        /**
         * Returns a map of UIDs to their corresponding property names.
         */
        [[nodiscard]] const std::map<UID, std::string>& getPropertiesNames() const;

        /**
         * Clears all properties.
         */
        void clear();
    };
} // namespace mindset

#endif // PROPERTIES_H
