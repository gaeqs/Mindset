//
// Created by gaeqs on 25/03/25.
//

#include <mindset/Versioned.h>

namespace mindset
{

    Versioned::Versioned() :
        _version(0)
    {
    }

    uint64_t Versioned::getVersion() const
    {
        return _version;
    }

    void Versioned::incrementVersion()
    {
        ++_version;
    }
} // namespace mindset
