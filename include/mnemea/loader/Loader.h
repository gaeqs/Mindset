//
// Created by gaeqs on 14/03/25.
//

#ifndef LOADER_H
#define LOADER_H

#include <filesystem>
#include <istream>
#include <string>

#include <hey/Observable.h>

#include <mnemea/Dataset.h>
#include <mnemea/loader/LoaderStatus.h>

namespace mnemea
{
    /**
     * Abstract class defining an interface for dataset loaders, allowing the loading of data into a Dataset instance.
     */
    class Loader : public hey::Observable<LoaderStatus>
    {
      public:
        /**
         * Loads data into the provided dataset.
         * @param dataset The dataset to populate.
         */
        virtual void load(Dataset& dataset) const = 0;

        /**
         * Adds a provider function for generating unique identifiers.
         * @param provider Function to generate UIDs.
         */
        virtual void addUIDProvider(std::function<UID()> provider);
    };

    class LoaderFactory
    {
      public:
        using FactoryResult = Result<std::unique_ptr<Loader>, std::string>;
        using FileProvider = std::function<std::optional<std::vector<std::string>>(std::filesystem::path)>;

        using FromPath = std::function<FactoryResult(FileProvider, const std::filesystem::path&)>;
        using FromLines = std::function<FactoryResult(FileProvider, const std::vector<std::string>&)>;
        using FromIstream = std::function<FactoryResult(FileProvider, std::istream&)>;
        using SupportFilter = std::function<bool(const std::string&)>;

      private:
        std::string _id;
        std::string _displayName;
        bool _providesUIDs;

        SupportFilter _supportFilter;

        FromPath _fromPath;
        FromLines _fromLines;
        FromIstream _fromIstream;

      public:
        /**
         * Constructs a LoaderFactory with the specified parameters.
         * @param id Unique identifier of the loader.
         * @param displayName Human-readable loader name.
         * @param providesUIDs Indicates if the loader generates UIDs.
         * @param supportFilter Function to filter supported file types.
         * @param fromPath Loader creator from file paths.
         * @param fromLines Loader creator from lines of text.
         * @param fromIstream Loader creator from streams.
         */
        LoaderFactory(std::string id, std::string displayName, bool providesUIDs, SupportFilter supportFilter,
                      FromPath fromPath = nullptr, FromLines fromLines = nullptr, FromIstream fromIstream = nullptr);

        /**
         * Retrieves the loader's unique identifier.
         */
        [[nodiscard]] std::string getId() const;

        /**
         * Retrieves the human-readable loader name.
         */
        [[nodiscard]] std::string getDisplayName() const;

        /**
         * Checks if the loader generates UIDs.
         */
        [[nodiscard]] bool providesUIDs() const;

        /**
         * Checks if the loader supports a specific file based on its filename.
         */
        [[nodiscard]] bool supportsFile(const std::string& filename) const;

        /**
         * Creates a Loader instance from a file path.
         */
        [[nodiscard]] FactoryResult create(FileProvider provider, const std::filesystem::path& path) const;

        /**
         * Creates a Loader instance from lines of text.
         */
        [[nodiscard]] FactoryResult create(FileProvider provider, const std::vector<std::string>& lines) const;

        /**
         * Creates a Loader instance from an input stream.
         */
        [[nodiscard]] FactoryResult create(FileProvider provider, std::istream& stream) const;
    };
} // namespace mnemea

#endif //LOADER_H
