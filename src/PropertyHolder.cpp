//
// Created by gaeqs on 11/03/2025.
//

#include <mindset/PropertyHolder.h>
#include <mindset/Properties.h>

namespace mindset
{
    PropertyHolder::PropertyHolder() = default;

    const std::unordered_map<UID, std::any>& PropertyHolder::getProperties() const
    {
        return _properties;
    }

    std::vector<std::pair<std::optional<std::string>, std::any>> PropertyHolder::getNamedProperties(
        const Properties& properties) const
    {
        std::vector<std::pair<std::optional<std::string>, std::any>> result;
        result.reserve(_properties.size());

        for (const auto& [uid, any] : _properties) {
            auto name = properties.getPropertyName(uid);
            result.emplace_back(name, any);
        }

        return result;
    }

    void PropertyHolder::setProperty(UID uid, std::any value)
    {
        incrementVersion();
        _properties[uid] = std::move(value);
    }

    std::optional<std::any> PropertyHolder::getPropertyAsAny(UID uid) const
    {
        auto it = _properties.find(uid);
        if (it == _properties.end()) {
            return {};
        }
        return it->second;
    }

    std::optional<std::any&> PropertyHolder::getPropertyAsAnyRef(UID uid)
    {
        auto it = _properties.find(uid);
        if (it == _properties.end()) {
            return {};
        }
        return it->second;
    }

    std::optional<const std::any&> PropertyHolder::getPropertyAsAnyRef(UID uid) const
    {
        auto it = _properties.find(uid);
        if (it == _properties.end()) {
            return {};
        }
        return it->second;
    }

    bool PropertyHolder::hasProperty(UID uid) const
    {
        return _properties.contains(uid);
    }

    bool PropertyHolder::deleteProperty(UID uid)
    {
        bool updated = _properties.erase(uid) > 0;
        if (updated) {
            incrementVersion();
        }
        return updated;
    }
} // namespace mindset
