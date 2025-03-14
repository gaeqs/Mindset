//
// Created by gaeqs on 11/03/2025.
//

#ifndef PROPERTYHOLDER_H
#define PROPERTYHOLDER_H

#include <any>
#include <optional>
#include <map>

#include <mnemea/UID.h>

namespace mnemea {
    class PropertyHolder {
        std::map<UID, std::any> _properties;

    public:
        PropertyHolder();

        void setPropertyAsAny(UID uid, std::any value);

        [[nodiscard]] std::optional<std::any> getPropertyAsAny(UID uid) const;

        [[nodiscard]] bool hasProperty(UID uid) const;

        bool deleteProperty(UID uid);

        template<typename T>
        [[nodiscard]] std::optional<T> getProperty(UID uid) const {
            auto optional = getPropertyAsAny(uid);
            if (!optional.has_value()) return {};
            T* v = std::any_cast<T>(&optional.value());
            if (v == nullptr) return {};
            return std::optional<T>(*v);
        }
    };
}

#endif //PROPERTYHOLDER_H
