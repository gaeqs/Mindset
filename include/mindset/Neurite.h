//
// Created by gaeqs on 11/03/2025.
//

#ifndef NEURITE_H
#define NEURITE_H

#include <mindset/Identifiable.h>
#include <mindset/PropertyHolder.h>

namespace mindset
{
    /**
     * The Neurite class represents a neuronal neurite, such as an axon or dendrite,
     * and can store additional properties.
     */
    class Neurite : public Identifiable, public PropertyHolder
    {
      public:
        /**
         * Constructs a Neurite with the given UID.
         * @param uid Unique identifier for the neurite.
         */
        explicit Neurite(UID uid);
    };
} // namespace mindset

#endif // NEURITE_H
