//
// Created by gaeqs on 14/03/25.
//

#ifndef LOADERREGISTRY_H
#define LOADERREGISTRY_H
#include <string>
#include <unordered_map>

#include <mnemea/loader/Loader.h>

namespace mnemea
{
    class LoaderRegistry
    {
        std::unordered_map<std::string, LoaderFactory> _factories;

      public:
        explicit LoaderRegistry(bool loadDefaults = true);

        bool add(LoaderFactory factory);

        bool remove(const std::string& id);

        void clear();

        std::optional<LoaderFactory> get(const std::string& name) const;

        [[nodiscard]] auto getAll() const
        {
            return _factories |
                   std::views::transform([](const auto& pair) -> const LoaderFactory& { return pair.second; });
        }
    };
} // namespace mnemea

#endif //LOADERREGISTRY_H
