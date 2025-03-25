//
// Created by gaeqs on 25/03/25.
//

#include <atomic>
#include <mnemea/Versioned.h>

namespace
{
    std::atomic_uint64_t VERSION_GENERATOR = 0;
}

namespace mnemea
{

    Versioned::Versioned() :
        _version(VERSION_GENERATOR++)
    {
    }
    uint64_t Versioned::getVersion() const
    {
        return _version;
    }
    void Versioned::setNewVersion()
    {
        _version = VERSION_GENERATOR++;
    }
} // namespace mnemea
