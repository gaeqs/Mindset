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

#ifndef MINDSET_MUTEXHOLDER_H
#define MINDSET_MUTEXHOLDER_H

#include <memory>
#include <mutex>
#include <shared_mutex>

namespace mindset
{

    /**
     * @class MutexHolder
     *
     * A RAII-based utility class to manage a `shared_mutex` for synchronization.
     *
     * Provides methods to acquire either read or write locks on an associated
     * `std::shared_mutex`. It ensures thread-safe access to shared resources by
     * allowing multiple readers or a single writer at a time.
     *
     * Elements extending this class shouldn't use the locks by themselves:
     * it is the user's responsibility to lock the resources by themselves.
     */
    class MutexHolder
    {
        std::unique_ptr<std::shared_mutex> _mutex;

      public:
        MutexHolder();

        MutexHolder(MutexHolder&& other) = default;
        MutexHolder& operator=(MutexHolder&& other) = default;

        MutexHolder(const MutexHolder&);
        MutexHolder& operator=(const MutexHolder&);

        /**
         * Acquires a read lock on the `shared_mutex` managed by the `MutexHolder`.
         *
         * This method returns an `std::shared_lock` to provide shared access to multiple
         * readers. It ensures that the underlying resource can be safely accessed by
         * concurrent threads in a read-only mode, as long as no writer lock is held.
         *
         * @return A `std::shared_lock` that holds a lock for the current scope.
         */
        std::shared_lock<std::shared_mutex> readLock() const;

        /**
         * Acquires a unique (write) lock on the associated `std::shared_mutex`.
         *
         * This method ensures exclusive access for a writer by preventing other
         * threads from acquiring either read or write locks on the same mutex
         * until the lock is released.
         *
         * @return A `std::unique_lock' representing the acquired write lock.
         */
        std::unique_lock<std::shared_mutex> writeLock() const;
    };

} // namespace mindset

#endif // MINDSET_MUTEXHOLDER_H
