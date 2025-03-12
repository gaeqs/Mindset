//
// Created by gaeqs on 11/03/2025.
//

#include <mnemea/PropertyHolder.h>

namespace mnemea {
    PropertyHolder::PropertyHolder() = default;

    void PropertyHolder::setPropertyAsAny(UID uid, std::any value) {
        _properties[uid] = std::move(value);
    }

    std::optional<std::any> PropertyHolder::getPropertyAsAny(UID uid) const {
        auto it = _properties.find(uid);
        if (it == _properties.end()) return {};
        return it->second;
    }

    bool PropertyHolder::hasProperty(UID uid) const {
        return _properties.contains(uid);
    }

    bool PropertyHolder::deleteProperty(UID uid) {
        return _properties.erase(uid) > 0;
    }
}
