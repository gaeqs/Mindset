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

#ifndef MORPHOLOGYTREE_H
#define MORPHOLOGYTREE_H

#include <ranges>
#include <unordered_map>
#include <optional>

#include <mindset/MorphologyTreeSection.h>

namespace mindset
{
    class Dataset;
    class Morphology;

    /**
     * Represents a hierarchical tree structure organizing morphological data into sections.
     * Each section can have child sections, enabling efficient traversal and management.
     */
    class MorphologyTree : public Versioned
    {
        std::unordered_map<UID, MorphologyTreeSection> _sections;
        std::optional<int64_t> _morphologyVersion;
        std::optional<UID> _root;

      public:
        /**
         * Constructs an empty MorphologyTree.
         */
        MorphologyTree();

        /**
         * Constructs a MorphologyTree from the given morphology and dataset.
         * @param morphology Pointer to the Morphology object; if null or invalid, creates an empty tree.
         * @param dataset Dataset containing properties necessary for building the tree.
         */
        MorphologyTree(const Morphology* morphology, const Dataset& dataset);

        /**
         * Gets the version of the associated morphology, if available.
         */
        [[nodiscard]] std::optional<int64_t> getMorphologyVersion() const;

        /**
         * Retrieves a mutable pointer to the root section, if exists.
         */
        [[nodiscard]] std::optional<MorphologyTreeSection*> getRoot();

        /**
         * Retrieves a const pointer to the root section, if exists.
         */
        [[nodiscard]] std::optional<const MorphologyTreeSection*> getRoot() const;

        /**
         * Retrieves a mutable pointer to a section identified by UID.
         */
        [[nodiscard]] std::optional<MorphologyTreeSection*> getSection(UID uid);

        /**
         * Retrieves a const pointer to a section identified by UID.
         */
        [[nodiscard]] std::optional<const MorphologyTreeSection*> getSection(UID uid) const;

        /**
         * Reserves space for a specified number of sections.
         */
        void reserveSpaceForSections(size_t amount);

        /**
         * Adds a new section to the tree.
         * @param section The section to add.
         * @param root If true, sets this section as the root, replacing any previous root.
         * @return A pair containing a pointer to the inserted section and a boolean indicating success.
         */
        std::pair<MorphologyTreeSection*, bool> addSection(MorphologyTreeSection section, bool root = false);

        /**
         * Removes the neurite identified by UID from the tree.
         * @param uid The UID of the neurite to remove.
         * @return True if successfully removed; false if neurite was not found.
         */
        bool removeNeurite(UID uid);

        /**
         * Gets the total number of sections in the tree.
         */
        [[nodiscard]] size_t getSectionsAmount() const;

        /**
         * Finds the section containing a specific neurite.
         */
        [[nodiscard]] std::optional<MorphologyTreeSection*> getSectionWithNeurite(UID neuriteId);

        /**
         * Finds the section containing a specific neurite.
         */
        [[nodiscard]] std::optional<const MorphologyTreeSection*> getSectionWithNeurite(UID neuriteId) const;

        /**
         * Performs a walk from the specified section up to the root, returning mutable pointers.
         * If the root is unreachable, returns the path until the break point.
         */
        std::vector<MorphologyTreeSection*> walkToRoot(UID sectionId);

        /**
         * Performs a walk from the specified section up to the root, returning const pointers.
         * If the root is unreachable, returns the path until the break point.
         */
        [[nodiscard]] std::vector<const MorphologyTreeSection*> walkToRoot(UID sectionId) const;

        /**
         * Performs a flat walk from the specified section up to the root, collecting neurite UIDs.
         * If the root is unreachable, returns the path collected until the break point.
         */
        [[nodiscard]] std::vector<UID> flatWalkToRoot(UID sectionId) const;

        /**
         * Returns a mutable view of all sections for iteration purposes.
         */
        [[nodiscard]] auto getSections()
        {
            return _sections | std::views::transform([](auto& pair) -> MorphologyTreeSection& { return pair.second; });
        }

        /**
         * Returns a const view of all sections for iteration purposes.
         */
        [[nodiscard]] auto getSections() const
        {
            return _sections |
                   std::views::transform([](const auto& pair) -> const MorphologyTreeSection& { return pair.second; });
        }
    };

} // namespace mindset

#endif // MORPHOLOGYTREE_H
