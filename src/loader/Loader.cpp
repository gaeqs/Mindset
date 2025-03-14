//
// Created by gaeqs on 14/03/25.
//

#include <mnemea/loader/Loader.h>

namespace mnemea {
    LoaderFactory::LoaderFactory(std::string id,
                                 std::string displayName,
                                 bool providesUIDs,
                                 SupportFilter supportFilter,
                                 FromPath fromPath,
                                 FromLines fromLines,
                                 FromIstream fromIstream)
        : _id(id),
          _displayName(displayName),
          _supportFilter(supportFilter),
          _providesUIDs(providesUIDs),
          _fromPath(fromPath),
          _fromLines(fromLines),
          _fromIstream(fromIstream) {}

    void Loader::addUIDProvider(std::function<UID()>) {}

    std::string LoaderFactory::getId() const {
        return _id;
    }

    std::string LoaderFactory::getDisplayName() const {
        return _displayName;
    }

    bool LoaderFactory::providesUIDs() const {
        return _providesUIDs;
    }

    bool LoaderFactory::supportsFile(const std::string& filename) const {
        return _supportFilter == nullptr ? false : _supportFilter(filename);
    }

    LoaderFactory::Result LoaderFactory::create(FileProvider provider, const std::filesystem::path& path) const {
        if (_fromPath == nullptr) {
            return {"This loader doesn't support paths."};
        }

        return _fromPath(std::move(provider), path);
    }

    LoaderFactory::Result LoaderFactory::create(FileProvider provider, const std::vector<std::string>& lines) const {
        if (_fromLines == nullptr) {
            return {"This loader doesn't support lines."};
        }

        return _fromLines(std::move(provider), lines);
    }

    LoaderFactory::Result LoaderFactory::create(FileProvider provider, std::istream& stream) const {
        if (_fromIstream == nullptr) {
            return {"This loader doesn't support streams."};
        }

        return _fromIstream(std::move(provider), stream);
    }
}
