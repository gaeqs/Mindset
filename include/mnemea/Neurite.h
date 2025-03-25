//
// Created by gaeqs on 11/03/2025.
//

#ifndef NEURITE_H
#define NEURITE_H
#include <mnemea/Identifiable.h>
#include <mnemea/PropertyHolder.h>

namespace mnemea
{
    class Neurite : public Identifiable, public PropertyHolder
    {
      public:
        explicit Neurite(UID uid);
    };
} // namespace mnemea

#endif //NEURITE_H
