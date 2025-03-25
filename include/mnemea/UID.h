//
// Created by gaeqs on 09/03/2025.
//

#ifndef UID_H
#define UID_H

#include <cstdint>

namespace mnemea
{
    /**
    * Unique identifier of an element.
    *
    * Identifiers must be unique for diferente elements of the same type,
    * but elements of different types may share UIDs.
    */
    using UID = uint32_t;
} // namespace mnemea

#endif //UID_H
