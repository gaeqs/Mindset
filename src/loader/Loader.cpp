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

#include <mindset/loader/Loader.h>

#include <utility>

namespace mindset
{

    Loader::Loader(const LoaderCreateInfo& info) :
        _info(info)
    {
    }

    const FileProvider& Loader::getFileProvider() const
    {
        return _info.fileProvider;
    }

    std::optional<const std::any*> Loader::getEnvironmentEntryAsAny(const std::string& value) const
    {
        auto it = _info.environment.find(value);
        if (it != _info.environment.end()) {
            return &it->second;
        }

        auto itDef = _info.defaultEnvironmentEntries.find(value);
        if (itDef != _info.defaultEnvironmentEntries.end()) {
            if (auto& opt = itDef->second.defaultValue; opt.has_value()) {
                return &opt.value();
            }
        }

        return {};
    }

    void Loader::addUIDProvider(std::function<UID()>)
    {
    }

    LoaderFactory::LoaderFactory(std::string id, std::string displayName, bool providesUIDs,
                                 const std::vector<LoaderEnvironmentEntry>& environmentEntries,
                                 SupportFilter supportFilter, FromPath fromPath, FromLines fromLines,
                                 FromIstream fromIstream) :
        _id(std::move(id)),
        _displayName(std::move(displayName)),
        _supportFilter(std::move(supportFilter)),
        _providesUIDs(providesUIDs),
        _fromPath(std::move(fromPath)),
        _fromLines(std::move(fromLines)),
        _fromIstream(std::move(fromIstream))
    {
        for (auto& entry : environmentEntries) {
            _environmentEntries.insert({entry.name, entry});
        }
    }

    std::string LoaderFactory::getId() const
    {
        return _id;
    }

    std::string LoaderFactory::getDisplayName() const
    {
        return _displayName;
    }

    bool LoaderFactory::providesUIDs() const
    {
        return _providesUIDs;
    }

    const std::map<std::string, LoaderEnvironmentEntry>& LoaderFactory::getEnvironmentEntries() const
    {
        return _environmentEntries;
    }

    bool LoaderFactory::supportsFile(const std::string& filename) const
    {
        return _supportFilter == nullptr ? false : _supportFilter(filename);
    }

    FactoryResult LoaderFactory::create(FileProvider provider, Environment environment,
                                        const std::filesystem::path& path) const
    {
        if (_fromPath == nullptr) {
            return {"This loader doesn't support paths."};
        }

        LoaderCreateInfo info{provider, environment, _environmentEntries};
        return _fromPath(info, path);
    }

    FactoryResult LoaderFactory::create(FileProvider provider, Environment environment,
                                        const std::vector<std::string>& lines) const
    {
        if (_fromLines == nullptr) {
            return {"This loader doesn't support lines."};
        }

        LoaderCreateInfo info{provider, environment, _environmentEntries};
        return _fromLines(info, lines);
    }

    FactoryResult LoaderFactory::create(FileProvider provider, Environment environment, std::istream& stream) const
    {
        if (_fromIstream == nullptr) {
            return {"This loader doesn't support streams."};
        }

        LoaderCreateInfo info{provider, environment, _environmentEntries};
        return _fromIstream(info, stream);
    }
} // namespace mindset
