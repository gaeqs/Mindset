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

    const std::string PROPERTY_SYNAPSE_PRE_NEURITE = "mindset:synapse_pre_neurite";
    const std::string PROPERTY_SYNAPSE_POST_NEURITE = "mindset:synapse_post_neurite";

    const std::string PROPERTY_SYNAPSE_PRE_POSITION = "mindset:synapse_pre_position";
    const std::string PROPERTY_SYNAPSE_POST_POSITION = "mindset:synapse_post_position";

    const std::string PROPERTY_SYNAPSE_DELAY = "mindset:synapse_delay";
    const std::string PROPERTY_SYNAPSE_CONDUCTANCE = "mindset:synapse_conductance";
    const std::string PROPERTY_SYNAPSE_UTILIZATION = "mindset:synapse_utilization";
    const std::string PROPERTY_SYNAPSE_DEPRESSION = "mindset:synapse_depression";
    const std::string PROPERTY_SYNAPSE_FACILITATION = "mindset:synapse_facilitation";
    const std::string PROPERTY_SYNAPSE_DECAY = "mindset:synapse_decay";
    const std::string PROPERTY_SYNAPSE_EFFICACY = "mindset:synapse_efficacy";

    const std::string PROPERTY_SIMULATION_VOLTAGE = "mindset:simulation_voltage";
    const std::string PROPERTY_SIMULATION_SPIKES = "mindset:simulation_spikes";

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

#endif // MINDSET_DEFAULTPARAMETERS_H
