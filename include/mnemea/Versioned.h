//
// Created by gaeqs on 25/03/25.
//

#ifndef VERSIONED_H
#define VERSIONED_H

#include <cstdint>

namespace mnemea
{

    class Versioned
    {
        uint64_t _version;

      public:
        Versioned();

        [[nodiscard]] uint64_t getVersion() const;

        void setNewVersion();
    };

} // namespace mnemea

#endif // VERSIONED_H
