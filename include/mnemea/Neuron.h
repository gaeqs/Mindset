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
    class Neuron : public Identifiable, public PropertyHolder
    {
        std::shared_ptr<Morphology> _morphology;

      public:
        explicit Neuron(UID uid, std::shared_ptr<Morphology> morphology = nullptr);

        [[nodiscard]] std::optional<Morphology*> getMorphology();

        [[nodiscard]] std::optional<const Morphology*> getMorphology() const;

        void setMorphology(std::shared_ptr<Morphology> morphology);
    };
} // namespace mnemea

#endif //NEURON_H
