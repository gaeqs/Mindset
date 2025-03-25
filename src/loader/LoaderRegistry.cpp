//
// Created by gaeqs on 14/03/25.
//

#include <mnemea/loader/BlueConfigLoader.h>
#include <mnemea/loader/LoaderRegistry.h>
#include <mnemea/loader/MorphoIOLoader.h>
#include <mnemea/loader/SnuddaLoader.h>
#include <mnemea/loader/SWCLoader.h>
#include <mnemea/loader/XMLLoader.h>

namespace mnemea
{
    LoaderRegistry::LoaderRegistry(bool loadDefaults)
    {
        if (loadDefaults) {
#ifdef MNEMEA_BRION
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
} // namespace mnemea
