//
// Created by gaeqs on 12/03/25.
//

#ifndef MORPHOLOGY_H
#define MORPHOLOGY_H

#include <ranges>

#include <mindset/Neurite.h>
#include <mindset/Soma.h>
#include <mindset/Versioned.h>
#include <mindset/MorphologyTree.h>

namespace mindset
{
    /**
     * The Morphology class represents the structural morphology of a neuron,
     * including its soma and neurites.
     */
    class Morphology : public Versioned
    {
        std::optional<Soma> _soma;
        std::unordered_map<UID, Neurite> _neurites;
        std::optional<MorphologyTree> _tree;

      public:
        /**
         * Constructs an empty Morphology object.
         */
        Morphology();

        /**
         * Returns a mutable pointer to the soma if it exists.
         */
        [[nodiscard]] std::optional<Soma*> getSoma();

        /**
         * Returns a const pointer to the soma if it exists.
         */
        [[nodiscard]] std::optional<const Soma*> getSoma() const;

        /**
         * Sets the soma of the neuron.
         * @param soma Soma to assign.
         */
        void setSoma(Soma soma);

        /**
         * Removes the soma from the morphology.
         */
        void clearSoma();

        /**
         * Returns a mutable pointer to the neurite with the specified UID.
         */
        [[nodiscard]] std::optional<Neurite*> getNeurite(UID uid);

        /**
         * Returns a const pointer to the neurite with the specified UID.
         */
        [[nodiscard]] std::optional<const Neurite*> getNeurite(UID uid) const;

        /**
         * Reserves space for a specified number of neurites.
         * @param amount Number of neurites to reserve space for.
         */
        void reserveSpaceForNeurites(size_t amount);

        /**
         * Adds a neurite to the morphology.
         * @param neurite Neurite to add.
         * @return Pair containing a pointer to the inserted neurite and a boolean indicating success.
         */
        std::pair<Neurite*, bool> addNeurite(Neurite neurite);

        /**
         * Removes the neurite that matches the given UID from the morphology.
         * @param uid UID of the neurite.
         * @return Whether a neurite was removed.
         */
        bool removeNeurite(UID uid);

        /**
         * Returns the amount of neurites this morphology has.
         * This doesn't include the soma!
         */
        [[nodiscard]] size_t getNeuritesAmount() const;

        std::optional<MorphologyTree*> getMorphologyTree();

        std::optional<const MorphologyTree*> getMorphologyTree() const;

        MorphologyTree* getOrCreateMorphologyTree(const Dataset& dataset);

        /**
         * Returns a view to iterate over all stored neurites' UIDs.
         * @returns A range view of UIDs.
         */
        [[nodiscard]] auto getNeuritesUIDs() const
        {
            return _neurites | std::views::keys;
        }

        /**
         * Returns a view to iterate over all stored neurites in a mutable context.
         * @return A range view of mutable neurites references.
         */
        [[nodiscard]] auto getNeurites()
        {
            return _neurites | std::views::transform([](auto& pair) -> Neurite* { return &pair.second; });
        }

        /**
         * Returns a view to iterate over all stored neurites in a const context.
         * @return A range view of const neurites references.
         */
        [[nodiscard]] auto getNeurites() const
        {
            return _neurites | std::views::transform([](const auto& pair) -> const Neurite* { return &pair.second; });
        }
    };
} // namespace mindset

#endif // MORPHOLOGY_H
