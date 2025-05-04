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

#ifndef LOADER_H
#define LOADER_H

#include <filesystem>
#include <istream>
#include <string>

#include <hey/Observable.h>

#include <mindset/Dataset.h>
#include <mindset/loader/LoaderStatus.h>

namespace mindset
{

    class Loader;

    struct LoaderEnvironmentEntry
    {
        std::string name;
        std::string displayName;
        std::type_index type;
        std::optional<std::any> defaultValue;
        std::optional<std::string> hint;
    };

    using FactoryResult = Result<std::unique_ptr<Loader>, std::string>;
    using FileProvider = std::function<std::optional<std::vector<std::string>>(std::filesystem::path)>;
    using Environment = std::unordered_map<std::string, std::any>;
    using DefaultEnvironmentEntries = std::map<std::string, LoaderEnvironmentEntry>;

    struct LoaderCreateInfo
    {
        FileProvider fileProvider;
        Environment environment;
        DefaultEnvironmentEntries defaultEnvironmentEntries;
    };

    /**
     * Abstract class defining an interface for dataset loaders, allowing the loading of data into a Dataset instance.
     */
    class Loader : public hey::Observable<LoaderStatus>
    {
        LoaderCreateInfo _info;

      public:
        Loader(const LoaderCreateInfo& info);

        const FileProvider& getFileProvider() const;

        std::optional<const std::any*> getEnvironmentEntryAsAny(const std::string& value) const;

        template<typename T>
        std::optional<const T*> getEnvironmentEntry(const std::string& value) const
        {
            auto optional = getEnvironmentEntryAsAny(value);
            if (!optional.has_value()) {
                return {};
            }
            const T* v = std::any_cast<T>(optional.value());
            if (v == nullptr) {
                return {};
            }
            return std::optional<const T*>(v);
        }

        template<typename T>
        T getEnvironmentEntryOr(const std::string& value, T other) const
        {
            auto optional = getEnvironmentEntryAsAny(value);
            if (!optional.has_value()) {
                return other;
            }
            const T* v = std::any_cast<T>(optional.value());
            if (v == nullptr) {
                return other;
            }
            return *v;
        }

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
        using FromPath = std::function<FactoryResult(const LoaderCreateInfo&, const std::filesystem::path&)>;
        using FromLines = std::function<FactoryResult(const LoaderCreateInfo&, const std::vector<std::string>&)>;
        using FromIstream = std::function<FactoryResult(const LoaderCreateInfo&, std::istream&)>;
        using SupportFilter = std::function<bool(const std::string&)>;

      private:
        std::string _id;
        std::string _displayName;
        bool _providesUIDs;
        std::map<std::string, LoaderEnvironmentEntry> _environmentEntries;

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
         * @param environmentEntries The variables that can be configured to modify the loader's behavior.
         * @param supportFilter Function to filter supported file types.
         * @param fromPath Loader creator from file paths.
         * @param fromLines Loader creator from lines of text.
         * @param fromIstream Loader creator from streams.
         */
        LoaderFactory(std::string id, std::string displayName, bool providesUIDs,
                      const std::vector<LoaderEnvironmentEntry>& environmentEntries, SupportFilter supportFilter,
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
         * Retrieves the environment entries the loader represented by this factory can use.
         * These entries represent configurable variables that can modify the behavior of a loader.
         * @return A constant reference to the collection of environment entries.
         */
        [[nodiscard]] const std::map<std::string, LoaderEnvironmentEntry>& getEnvironmentEntries() const;

        /**
         * Checks if the loader supports a specific file based on its filename.
         */
        [[nodiscard]] bool supportsFile(const std::string& filename) const;

        /**
         * Creates a Loader instance from a file path.
         */
        [[nodiscard]] FactoryResult create(FileProvider provider, Environment environment,
                                           const std::filesystem::path& path) const;

        /**
         * Creates a Loader instance from lines of text.
         */
        [[nodiscard]] FactoryResult create(FileProvider provider, Environment environment,
                                           const std::vector<std::string>& lines) const;

        /**
         * Creates a Loader instance from an input stream.
         */
        [[nodiscard]] FactoryResult create(FileProvider provider, Environment environment, std::istream& stream) const;
    };
} // namespace mindset

#endif // LOADER_H
