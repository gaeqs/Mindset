//
// Created by gaeqs on 12/03/25.
//

#ifndef DEFAULTPARAMETERS_H
#define DEFAULTPARAMETERS_H

namespace mnemea {

    const std::string PROPERTY_PARENT = "mnemea:parent";
    const std::string PROPERTY_RADIUS = "mnemea:radius";
    const std::string PROPERTY_POSITION = "mnemea:position";
    const std::string PROPERTY_NEURITE_TYPE = "mnemea:neurite_type";
    const std::string PROPERTY_TRANSFORM = "mnemea:transform";
    const std::string PROPERTY_COLUMN = "mnemea:column";
    const std::string PROPERTY_MINI_COLUMN = "mnemea:mini_column";
    const std::string PROPERTY_LAYER = "mnemea:layer";
    const std::string PROPERTY_NAME = "mnemea:name";

    enum NeuriteType : uint8_t {
        UNDEFINED = 0,
        SOMA = 1,
        AXON = 2,
        BASAL_DENDRITE = 3,
        APICAL_DENDRITE = 4,
        CUSTOM = 5,
        UNSPECIFIED_NEURITE = 6,
        GLIA_PROCESSES = 7
    };
}

#endif //DEFAULTPARAMETERS_H
