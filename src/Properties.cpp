//
// Created by gaeqs on 10/03/2025.
//

#include <mindset/Properties.h>

#include <ranges>

namespace mindset
{
    Properties::Properties() = default;

    UID Properties::defineProperty(std::string name)
    {
        if (auto found = getPropertyUID(name); found.has_value()) {
            return found.value();
        }

        UID maxId = 0;
        for (auto id : _properties | std::views::values) {
            maxId = std::max(id, maxId);
        }

        UID id = maxId + 1;
        _properties[name] = id;
        _propertiesNames[id] = std::move(name);

        return id;
    }

    void Properties::defineProperty(std::string name, UID id)
    {
        _properties[name] = id;
        _propertiesNames[id] = std::move(name);
    }

    bool Properties::isPropertyDefined(const std::string& name) const
    {
        return _properties.contains(name);
    }

    std::optional<UID> Properties::getPropertyUID(const std::string& name) const
    {
        auto it = _properties.find(name);
        if (it == _properties.end()) {
            return {};
        }
        return it->second;
    }

    const std::map<std::string, UID>& Properties::getPropertiesUIDs() const
    {
        return _properties;
    }

    const std::map<UID, std::string>& Properties::getPropertiesNames() const
    {
        return _propertiesNames;
    }
} // namespace mindset
