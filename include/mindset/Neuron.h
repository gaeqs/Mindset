// Copyright (c) 2025. VG-Lab/URJC.
//
// Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
//
// This file is part of Mindset <https://gitlab.gmrv.es/g.rial/mindset>
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 3.0 as published
// by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#ifndef NEURON_H
#define NEURON_H

#include <memory>

#include <mindset/MutexHolder.h>
#include <mindset/Identifiable.h>
#include <mindset/PropertyHolder.h>
#include <mindset/Morphology.h>

namespace mindset
{
    /**
     * Represents a neuron, holding a unique identifier, properties, and optional morphology.
     *
     * This representation usually stores atemporal data.
     */
    class Neuron : public Identifiable, public PropertyHolder, public MutexHolder
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
} // namespace mindset

#endif // NEURON_H
