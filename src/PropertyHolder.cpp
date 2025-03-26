//
// Created by gaeqs on 11/03/2025.
//

#include <mindset/PropertyHolder.h>

namespace mindset
{
    PropertyHolder::PropertyHolder() = default;

    void PropertyHolder::setPropertyAsAny(UID uid, std::any value)
    {
        setNewVersion();
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

    bool PropertyHolder::hasProperty(UID uid) const
    {
        return _properties.contains(uid);
    }

    bool PropertyHolder::deleteProperty(UID uid)
    {
        bool updated = _properties.erase(uid) > 0;
        if (updated) {
            setNewVersion();
        }
        return updated;
    }
} // namespace mindset
