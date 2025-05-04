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

#ifndef MORPHOLOGYTREESECTION_H
#define MORPHOLOGYTREESECTION_H

#include <span>
#include <unordered_set>

#include <mindset/Identifiable.h>
#include <mindset/PropertyHolder.h>

namespace mindset
{

    /**
     * Represents a section within a MorphologyTree, holding neurite identifiers and hierarchical relationships.
     */
    class MorphologyTreeSection : public Identifiable, public PropertyHolder
    {
        std::vector<UID> _neurites;
        std::optional<UID> _parentSection;
        std::unordered_set<UID> _childSections;

      public:
        /**
         * Constructs a MorphologyTreeSection identified by a unique UID.
         */
        explicit MorphologyTreeSection(UID uid);

        /**
         * Constructs a MorphologyTreeSection with initial neurite identifiers.
         */
        explicit MorphologyTreeSection(UID uid, std::initializer_list<UID> neurites);

        /**
         * Constructs a MorphologyTreeSection from a span of neurite identifiers.
         */
        explicit MorphologyTreeSection(UID uid, std::span<const UID> neurites);

        /**
         * Retrieves a const span containing all neurite identifiers in this section.
         */
        [[nodiscard]] std::span<const UID> getNeurites() const;

        /**
         * Checks whether a neurite identified by neuriteId exists within this section.
         */
        [[nodiscard]] bool containsNeurite(UID neuriteId) const;

        /**
         * Replaces all current neurites with the provided neurite identifiers.
         * @param uids Span of neurite identifiers to set.
         */
        void redefineNeurites(std::span<const UID> uids);

        /**
         * Adds a neurite at the end of this section.
         * @param uid The neurite.
         */
        void addNeurite(UID uid);

        /**
         * Returns the count of neurites within this section.
         */
        [[nodiscard]] size_t getNeuritesCount() const;

        /**
         * Retrieves the UID of the first neurite. Returns std::nullopt if the section has no neurites.
         */
        [[nodiscard]] std::optional<UID> getFistNeurite();

        /**
         * Retrieves the UID of the last neurite. Returns std::nullopt if the section has no neurites.
         */
        [[nodiscard]] std::optional<UID> getLastNeurite();

        /**
         * Retrieves the UID of the parent section. Returns std::nullopt if no parent is set.
         */
        [[nodiscard]] std::optional<UID> getParentSection() const;

        /**
         * Retrieves a const reference to the set of child section identifiers.
         */
        [[nodiscard]] const std::unordered_set<UID>& getChildSections() const;

        /**
         * Sets the parent section for this section.
         * @param parent UID of the new parent section.
         */
        void setParentSection(UID parent);

        /**
         * Adds a child section identifier.
         * @param child UID of the child section to add.
         * @return True if the child was successfully added; false if it already existed.
         */
        bool addChildSection(UID child);

        /**
         * Removes the parent section for this section.
         */
        void removeParentSection();

        /**
         * Removes a child section identifier.
         * @param child UID of the child section to remove.
         * @return True if the child was successfully removed; false if it didn't exist.
         */
        bool removeChildSection(UID child);
    };

} // namespace mindset

#endif // MORPHOLOGYTREESECTION_H
