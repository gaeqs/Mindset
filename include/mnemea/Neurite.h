//
// Created by gaeqs on 11/03/2025.
//

#ifndef NEURITE_H
#define NEURITE_H
#include <mnemea/Identifiable.h>
#include <mnemea/PropertyHolder.h>

namespace mnemea
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
} // namespace mnemea

#endif //NEURITE_H
