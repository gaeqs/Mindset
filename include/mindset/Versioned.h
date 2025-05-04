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

#ifndef VERSIONED_H
#define VERSIONED_H

#include <atomic>
#include <cstdint>

namespace mindset
{
    /**
     * Provides versioning capabilities for derived objects.
     * Each object has a version number that increments with every update.
     * Remember that this versioning system is not automatic and doesn't track
     * deep modifications.
     * If you want to force a new version, you can use setNerVersion() manually.
     */
    class Versioned
    {
        std::uint64_t _version;

      public:
        /**
         * Constructs a Versioned object with an initial unique version number.
         */
        Versioned();

        /**
         * Retrieves the current version of the object.
         * @return Current version number.
         */
        [[nodiscard]] uint64_t getVersion() const;

        /**
         * Sets a new unique version number, typically after modification.
         */
        void incrementVersion();
    };

} // namespace mindset

#endif // VERSIONED_H
