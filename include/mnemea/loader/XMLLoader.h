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

#include <mnemea/Dataset.h>
#include <mnemea/UID.h>
#include <mnemea/util/NeuronTransform.h>
#include <mnemea/loader/Loader.h>

namespace mnemea {


    static const std::string XML_LOADER_ID = "mnemea:loader_xml";
    static const std::string XML_LOADER_NAME = "XML";

    /**
    * This loader loads XML Scene files.
    */
    class XMLLoader : public Loader {
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

        void load(Dataset& dataset) const override;

        static LoaderFactory createFactory();
    };
}

#endif //XMLLOADER_H
