//
// Created by gaeqs on 12/03/25.
//

#ifndef MINDSET_DEFAULTPARAMETERS_H
#define MINDSET_DEFAULTPARAMETERS_H

#include <string>
#include <cstdint>

namespace mindset
{

    const std::string PROPERTY_PARENT = "mindset:parent";
    const std::string PROPERTY_RADIUS = "mindset:radius";
    const std::string PROPERTY_POSITION = "mindset:position";
    const std::string PROPERTY_NEURITE_TYPE = "mindset:neurite_type";
    const std::string PROPERTY_TRANSFORM = "mindset:transform";
    const std::string PROPERTY_COLUMN = "mindset:column";
    const std::string PROPERTY_MINI_COLUMN = "mindset:mini_column";
    const std::string PROPERTY_LAYER = "mindset:layer";
    const std::string PROPERTY_NAME = "mindset:name";
    const std::string PROPERTY_SYNAPSE_PRE_SECTION = "mindset:synapse_pre_section";
    const std::string PROPERTY_SYNAPSE_POST_SECTION = "mindset:synapse_post_section";

    enum class NeuriteType : uint8_t
    {
        UNDEFINED = 0,
        SOMA = 1,
        AXON = 2,
        BASAL_DENDRITE = 3,
        APICAL_DENDRITE = 4,
        CUSTOM = 5,
        UNSPECIFIED_NEURITE = 6,
        GLIA_PROCESSES = 7
    };
} // namespace mindset

#endif //MINDSET_DEFAULTPARAMETERS_H
