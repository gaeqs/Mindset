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

namespace mnemea {
    class Loader : public hey::Observable<LoaderStatus> {
        virtual void load(Dataset& dataset) const = 0;

        virtual void addUIDProvider(std::function<UID()> provider);
    };

    class LoaderFactory {
    public:
        using Result = Result<std::unique_ptr<Loader>, std::string>;
        using FileProvider = std::function<std::optional<std::vector<std::string>>(std::filesystem::__cxx11::path)>;

        using FromPath = std::function<Result(FileProvider, const std::filesystem::path&)>;
        using FromLines = std::function<Result(FileProvider, const std::vector<std::string>&)>;
        using FromIstream = std::function<Result(FileProvider, std::istream&)>;
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
        LoaderFactory(std::string id,
                      std::string displayName,
                      bool providesUIDs,
                      SupportFilter supportFilter,
                      FromPath fromPath = nullptr,
                      FromLines fromLines = nullptr,
                      FromIstream fromIstream = nullptr);

        [[nodiscard]] std::string getId() const;

        [[nodiscard]] std::string getDisplayName() const;

        [[nodiscard]] bool providesUIDs() const;

        bool supportsFile(const std::string& filename) const;

        Result create(FileProvider provider, const std::filesystem::path& path) const;

        Result create(FileProvider provider, const std::vector<std::string>& lines) const;

        Result create(FileProvider provider, std::istream& stream) const;
    };
}

#endif //LOADER_H
