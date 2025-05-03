//
// Created by gaeqs on 3/05/25.
//

#include <mindset/MutexHolder.h>

namespace mindset
{

    MutexHolder::MutexHolder() :
        _mutex(std::make_unique<std::shared_mutex>())
    {
    }

    MutexHolder::MutexHolder(const MutexHolder&) :
        _mutex(std::make_unique<std::shared_mutex>())
    {
    }

    MutexHolder& MutexHolder::operator=(const MutexHolder&)
    {
        // Use the same mutex.
        return *this;
    }

    std::shared_lock<std::shared_mutex> MutexHolder::readLock() const
    {
        return std::shared_lock(*_mutex);
    }

    std::unique_lock<std::shared_mutex> MutexHolder::writeLock() const
    {
        return std::unique_lock(*_mutex);
    }
} // namespace mindset
