//
// Created by gaeqs on 11/03/2025.
//

#ifndef PROPERTYHOLDER_H
#define PROPERTYHOLDER_H

#include <any>
#include <optional>
#include <map>

#include <mnemea/Versioned.h>
#include <mnemea/UID.h>

namespace mnemea
{
    /**
     * Holds a set of properties, each associated with a UID and stored as any type.
     */
    class PropertyHolder : public Versioned
    {
        std::map<UID, std::any> _properties;

      public:

        /**
        * Creates a new property holder.
        */
        PropertyHolder();

        /**
         * Sets a property value by UID.
         * @param uid UID of the property.
         * @param value Property value (stored as std::any).
         */
        void setPropertyAsAny(UID uid, std::any value);

        /**
         * Retrieves the property value as std::any, if present.
         * @param uid UID of the property.
         */
        [[nodiscard]] std::optional<std::any> getPropertyAsAny(UID uid) const;

        /**
         * Checks if the property exists.
         * @param uid UID of the property.
         */
        [[nodiscard]] bool hasProperty(UID uid) const;

        /**
         * Deletes a property by UID.
         * @param uid UID of the property.
         * @return True if deleted successfully, false otherwise.
         */
        bool deleteProperty(UID uid);

        /**
         * Retrieves the property as a specific type, if present.
         * @tparam T Expected type of the property.
         * @param uid UID of the property.
         */
        template<typename T>
        [[nodiscard]] std::optional<T> getProperty(UID uid) const
        {
            auto optional = getPropertyAsAny(uid);
            if (!optional.has_value()) {
                return {};
            }
            T* v = std::any_cast<T>(&optional.value());
            if (v == nullptr) {
                return {};
            }
            return std::optional<T>(*v);
        }
    };
} // namespace mnemea

#endif //PROPERTYHOLDER_H
