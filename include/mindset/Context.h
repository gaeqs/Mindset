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


#ifndef CONTEXT_H
#define CONTEXT_H

namespace mindset
{

    class Context
    {
      public:
        virtual ~Context() = default;

        /**
         * Retrieves a mutable reference to the dataset's properties.
         * @return Reference to the properties object.
         */
        [[nodiscard]] virtual Properties& getProperties() = 0;

        /**
         * Retrieves a const reference to the dataset's properties.
         * @return Const reference to the properties object.
         */
        [[nodiscard]] virtual const Properties& getProperties() const = 0;
    };

} // namespace mindset

#endif // CONTEXT_H
