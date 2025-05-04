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

    std::optional<std::any*> PropertyHolder::getPropertyAsAnyPtr(UID uid)
    {
        auto it = _properties.find(uid);
        if (it == _properties.end()) {
            return {};
        }
        return &it->second;
    }

    std::optional<const std::any*> PropertyHolder::getPropertyAsAnyPtr(UID uid) const
    {
        auto it = _properties.find(uid);
        if (it == _properties.end()) {
            return {};
        }
        return &it->second;
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
