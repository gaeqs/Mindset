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
    /**
     * Registry maintaining available LoaderFactory instances, enabling easy retrieval and management of loaders.
     */
    class LoaderRegistry
    {
        std::unordered_map<std::string, LoaderFactory> _factories;

      public:
        /**
         * Constructs a LoaderRegistry and optionally loads default loaders.
         * @param loadDefaults If true, default loaders are added upon initialization.
         */
        explicit LoaderRegistry(bool loadDefaults = true);

        /**
         * Adds a LoaderFactory to the registry.
         * @param factory The loader factory to add.
         * @return True if successfully added; false if already exists.
         */
        bool add(LoaderFactory factory);

        /**
         * Removes a LoaderFactory from the registry by its ID.
         * @param id Identifier of the loader factory to remove.
         * @return True if successfully removed; false if it did not exist.
         */
        bool remove(const std::string& id);

        /**
         * Clears all loader factories from the registry.
         */
        void clear();

        /**
         * Retrieves a LoaderFactory by name, if it exists.
         * @param name Identifier of the loader factory.
         */
        std::optional<LoaderFactory> get(const std::string& name) const;

        /**
         * Provides a const range view of all registered loader factories.
         */
        [[nodiscard]] auto getAll() const
        {
            return _factories |
                   std::views::transform([](const auto& pair) -> const LoaderFactory& { return pair.second; });
        }
    };
} // namespace mnemea

#endif //LOADERREGISTRY_H
