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
     * Collection of temporal data.
     */
    class Activity : public Identifiable, public PropertyHolder
    {
    public:
        explicit Activity(UID uid);
    };

} // namespace mindset

#endif // SIMULATION_H
