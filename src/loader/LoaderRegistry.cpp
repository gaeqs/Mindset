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

#include <mindset/loader/BlueConfigLoader.h>
#include <mindset/loader/LoaderRegistry.h>
#include <mindset/loader/MorphoIOLoader.h>
#include <mindset/loader/SnuddaLoader.h>
#include <mindset/loader/SWCLoader.h>
#include <mindset/loader/XMLLoader.h>

namespace mindset
{
    LoaderRegistry::LoaderRegistry(bool loadDefaults)
    {
        if (loadDefaults) {
#ifdef MINDSET_BRION
            add(BlueConfigLoader::createFactory());
            add(MorphoIOLoader::createFactory());
#endif
            add(SWCLoader::createFactory());
            add(XMLLoader::createFactory());
            add(SnuddaLoader::createFactory());
        }
    }

    bool LoaderRegistry::add(LoaderFactory factory)
    {
        return _factories.emplace(factory.getId(), factory).second;
    }

    bool LoaderRegistry::remove(const std::string& id)
    {
        return _factories.erase(id) > 0;
    }

    void LoaderRegistry::clear()
    {
        _factories.clear();
    }

    std::optional<LoaderFactory> LoaderRegistry::get(const std::string& name) const
    {
        auto it = _factories.find(name);
        if (it == _factories.end()) {
            return {};
        }
        return it->second;
    }
} // namespace mindset
