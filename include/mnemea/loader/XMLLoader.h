//
// Created by gaeqs on 13/03/25.
//

#ifndef XMLLOADER_H
#define XMLLOADER_H
#include <filesystem>
#include <functional>
#include <string>
#include <optional>

#include <pugixml.hpp>
#include <hey/Observable.h>

#include <mnemea/Dataset.h>
#include <mnemea/UID.h>
#include <mnemea/util/NeuronTransform.h>
#include <mnemea/loader/LoaderStatus.h>

namespace mnemea {
    class XMLLoader : public hey::Observable<LoaderStatus> {
        struct XMLNeuron {
            UID id;
            std::optional<UID> column;
            std::optional<UID> miniColumn;
            std::optional<UID> layer;
            std::optional<std::string> neuronType;
            std::optional<NeuronTransform> transform;
            Node* node;
        };

    public:
        using FileProvider = std::function<std::optional<std::vector<std::string>>(std::filesystem::path)>;

    private:
        FileProvider _fileProvider;
        pugi::xml_document _doc;
        bool _valid;

    public:
        XMLLoader(FileProvider provider, const void* data, size_t size);

        XMLLoader(FileProvider provider, std::istream& stream);

        XMLLoader(FileProvider provider, std::filesystem::path path);

        void load(Dataset& dataset) const;
    };
}

#endif //XMLLOADER_H
