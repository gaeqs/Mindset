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

#ifndef SIMULATION_H
#define SIMULATION_H

#include <mindset/MutexHolder.h>
#include <mindset/Identifiable.h>
#include <mindset/PropertyHolder.h>

namespace mindset
{

    /**
     * Collection of temporal data.
     */
    class Activity : public Identifiable, public PropertyHolder, public MutexHolder
    {
      public:
        explicit Activity(UID uid);
    };

} // namespace mindset

#endif // SIMULATION_H
