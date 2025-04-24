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

#include <mindset/Dataset.h>
#include <mindset/UID.h>
#include <mindset/util/NeuronTransform.h>
#include <mindset/loader/Loader.h>

namespace mindset
{

    static const std::string XML_LOADER_ID = "mindset:loader_xml";
    static const std::string XML_LOADER_NAME = "XML";

    /**
    * This loader loads XML Scene files.
    */
    class XMLLoader : public Loader
    {
        struct XMLNeuron
        {
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
        pugi::xml_document _doc;
        bool _valid;

      public:
        XMLLoader(const LoaderCreateInfo& info, const void* data, size_t size);

        XMLLoader(const LoaderCreateInfo& info, std::istream& stream);

        XMLLoader(const LoaderCreateInfo& info, std::filesystem::path path);

        void load(Dataset& dataset) const override;

        static LoaderFactory createFactory();
    };
} // namespace mindset

#endif //XMLLOADER_H
