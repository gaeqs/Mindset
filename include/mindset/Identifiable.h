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

#ifndef IDENTIFIABLE_H
#define IDENTIFIABLE_H

#include <functional>

#include <mindset/UID.h>

namespace mindset
{
    /**
     * The Identifiable class provides a unique identifier to objects.
     */
    class Identifiable
    {
        UID _id;

      public:
        /**
         * Constructs an Identifiable object with a given UID.
         * @param id Unique identifier.
         */
        explicit Identifiable(UID id);

        /**
         * Returns the UID of the object.
         */
        [[nodiscard]] UID getUID() const;

        /**
         * Sets the UID of the object.
         * @param uid New UID to assign.
         */
        void setUID(UID uid);

        bool operator==(const Identifiable& other) const;

        bool operator!=(const Identifiable& other) const;
    };
} // namespace mindset

template<typename T>
    requires std::is_base_of_v<mindset::Identifiable, T>
struct std::hash<T>
{
    size_t operator()(const T& s) const noexcept
    {
        return std::hash<mindset::UID>()(s.getUID());
    }
};

#endif // IDENTIFIABLE_H
