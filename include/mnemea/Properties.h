//
// Created by gaeqs on 10/03/2025.
//

#ifndef PROPERTIES_H
#define PROPERTIES_H

#include <map>
#include <string>
#include <optional>

#include <mnemea/UID.h>

namespace mnemea {
    class Properties {
        std::map<std::string, UID> _properties;
        std::map<UID, std::string> _propertiesNames;

    public:
        Properties();

        UID defineProperty(std::string name);

        void defineProperty(std::string name, UID id);

        [[nodiscard]] bool isPropertyDefined(const std::string& name) const;

        [[nodiscard]] std::optional<UID> getPropertyUID(const std::string& name) const;

        [[nodiscard]] const std::map<std::string, UID>& getPropertiesUIDs() const;

        [[nodiscard]] const std::map<UID, std::string>& getPropertiesNames() const;
    };
}

#endif //PROPERTIES_H
