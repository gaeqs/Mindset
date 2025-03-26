//
// Created by gaeqs on 09/03/2025.
//

#ifndef NEURON_H
#define NEURON_H

#include <memory>
#include <mnemea/Identifiable.h>
#include <mnemea/PropertyHolder.h>
#include <mnemea/Morphology.h>

namespace mnemea
{
    /**
     * Represents a neuron, holding a unique identifier, properties, and optional morphology.
     */
    class Neuron : public Identifiable, public PropertyHolder
    {
        std::shared_ptr<Morphology> _morphology;

      public:
        /**
         * Constructs a Neuron with the given UID and optional morphology.
         * @param uid Unique identifier for the neuron.
         * @param morphology Optional shared pointer to the morphology.
         */
        explicit Neuron(UID uid, std::shared_ptr<Morphology> morphology = nullptr);

        /**
         * Retrieves a mutable pointer to the neuron's morphology, if available.
         */
        [[nodiscard]] std::optional<Morphology*> getMorphology();

        /**
         * Retrieves a const pointer to the neuron's morphology, if available.
         */
        [[nodiscard]] std::optional<const Morphology*> getMorphology() const;

        /**
         * Sets or updates the neuron's morphology.
         * @param morphology Shared pointer to the new morphology.
         */
        void setMorphology(std::shared_ptr<Morphology> morphology);
    };
} // namespace mnemea

#endif //NEURON_H
