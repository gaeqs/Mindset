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

#include <mindset/Identifiable.h>

namespace mindset
{
    Identifiable::Identifiable(UID id) :
        _id(id)
    {
    }

    UID Identifiable::getUID() const
    {
        return _id;
    }

    void Identifiable::setUID(UID uid)
    {
        _id = uid;
    }

    bool Identifiable::operator==(const Identifiable& other) const
    {
        return _id == other._id;
    }

    bool Identifiable::operator!=(const Identifiable& other) const
    {
        return _id != other._id;
    }
} // namespace mindset
