//
// Created by gaeqs on 10/03/2025.
//

#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <map>
#include <string>
#include <optional>

#include <mnemea/UID.h>

namespace mnemea
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
         * Retrieves the UID of a property by name, if defined.
         * @param name Name of the property.
         */
        [[nodiscard]] std::optional<UID> getPropertyUID(const std::string& name) const;

        /**
         * Returns a map of property names to their UIDs.
         */
        [[nodiscard]] const std::map<std::string, UID>& getPropertiesUIDs() const;

        /**
         * Returns a map of UIDs to their corresponding property names.
         */
        [[nodiscard]] const std::map<UID, std::string>& getPropertiesNames() const;
    };
} // namespace mnemea

#endif //PROPERTIES_H
