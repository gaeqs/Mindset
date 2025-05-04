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

    bool Properties::removeProperty(const std::string& name)
    {
        auto optional = getPropertyUID(name);
        if (!optional) {
            return false;
        }
        _propertiesNames.erase(optional.value());
        return _properties.erase(name) > 0;
    }

    std::optional<UID> Properties::getPropertyUID(const std::string& name) const
    {
        auto it = _properties.find(name);
        if (it == _properties.end()) {
            return {};
        }
        return it->second;
    }

    std::optional<std::string> Properties::getPropertyName(UID uid) const
    {        auto it = _propertiesNames.find(uid);
        if (it == _propertiesNames.end()) {
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

    void Properties::clear()
    {
        _propertiesNames.clear();
        _properties.clear();
    }
} // namespace mindset
