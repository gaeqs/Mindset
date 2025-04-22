//
// Created by gaeqs on 22/04/25.
//

#ifndef SIMULATION_H
#define SIMULATION_H

#include <mindset/Identifiable.h>
#include <mindset/PropertyHolder.h>

namespace mindset
{

    /**
     * Collection of data related to a simulation.
     */
    class Simulation : public Identifiable, public PropertyHolder
    {
        explicit Simulation(UID uid);
    };

} // namespace mindset

#endif // SIMULATION_H
