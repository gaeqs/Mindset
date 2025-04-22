//
// Created by gaeqs on 11/03/2025.
//

#ifndef PROPERTYHOLDER_H
#define PROPERTYHOLDER_H

#include <any>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <mindset/Versioned.h>
#include <mindset/UID.h>

namespace mindset
{
    class Properties;

    /**
     * Holds a set of properties, each associated with a UID and stored as any type.
     */
    class PropertyHolder : public Versioned
    {
        std::unordered_map<UID, std::any> _properties;

      public:
        /**
         * Creates a new property holder.
         */
        PropertyHolder();

        /**
         * Returns a reference to the property map.
         */
        const std::unordered_map<UID, std::any>& getProperties() const;

        /**
         * Returns a new vector holding all the properties of this holder with their respective names.
         */
        std::vector<std::pair<std::optional<std::string>, std::any>> getNamedProperties(
            const Properties& properties) const;

        /**
         * Sets a property value by UID.
         * @param uid UID of the property.
         * @param value Property value (stored as std::any).
         */
        void setProperty(UID uid, std::any value);

        /**
         * Retrieves the property value as std::any, if present.
         * @param uid UID of the property.
         */
        [[nodiscard]] std::optional<std::any> getPropertyAsAny(UID uid) const;

        [[nodiscard]] std::optional<std::any*> getPropertyAsAnyPtr(UID uid);

        [[nodiscard]] std::optional<const std::any*> getPropertyAsAnyPtr(UID uid) const;

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

        /**
         * Retrieves the property as a specific type's pointer, if present.
         * @tparam T Expected type of the property.
         * @param uid UID of the property.
         */
        template<typename T>
        [[nodiscard]] std::optional<T*> getPropertyPtr(UID uid)
        {
            auto optional = getPropertyAsAnyPtr(uid);
            if (!optional.has_value()) {
                return {};
            }
            T* v = std::any_cast<T>(optional.value());
            if (v == nullptr) {
                return {};
            }
            return std::optional<T*>(v);
        }

        /**
         * Retrieves the property as a specific type's pointer, if present.
         * @tparam T Expected type of the property.
         * @param uid UID of the property.
         */
        template<typename T>
        [[nodiscard]] std::optional<const T*> getPropertyPtr(UID uid) const
        {
            auto optional = getPropertyAsAnyPtr(uid);
            if (!optional.has_value()) {
                return {};
            }
            const T* v = std::any_cast<T>(optional.value());
            if (v == nullptr) {
                return {};
            }
            return std::optional<const T*>(v);
        }
    };
} // namespace mindset

#endif // PROPERTYHOLDER_H
