//
// Created by gaeqs on 09/03/2025.
//

#include <mnemea/Identifiable.h>

namespace mnemea
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
} // namespace mnemea
