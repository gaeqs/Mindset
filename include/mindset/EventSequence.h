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

#ifndef EVENTSEQUENCE_H
#define EVENTSEQUENCE_H

#include <chrono>
#include <set>
#include <ranges>

#include <mindset/UID.h>
#include <mindset/Versioned.h>

namespace mindset
{

    /**
     * @brief A container storing Event in a time sequence.
     *
     * This variable holds the set of all events within an EventSequence. Events
     * are stored in chronological order based on their `timepoint`.
     *
     * This container allows multiple events with the same `timepoint` to coexist.
     *
     * @details
     * - Each Event in the multiset contains a unique identifier (UID), a
     *   timepoint representing the event's timestamp, and a value of type `Value`
     *   representing the event's payload.
     * - The container ensures that events are sorted in ascending order
     *   according to their timepoint, facilitating operations like accessing the
     *   earliest or latest event, calculating duration, and retrieving events
     *   within specific time ranges.
     */
    template<typename Value>
    class EventSequence : public Versioned
    {
        /**
         * @brief Represents an event with associated data and behaviors.
         *
         * The Event class is designed to encapsulate all properties and operations
         * related to a specific occurrence or action in a system. It can store event
         * details, handle metadata, and provide functionality for managing the event's
         * lifecycle.
         */
      public:
        struct Event
        {
            /**
             * The identifier of the element associated with this event.
             * It can be defined as the element to whom this event happened.
             */
            UID uid;

            /**
             * Represents a specific time point associated with an event.
             *
             * It indicates the precise temporal location of an event within the sequence.
             * It is used as the primary sorting criterion for events and allows
             * accurate timing and ordering of events.
             */
            std::chrono::nanoseconds timepoint;

            /**
             * Represents the value hold by this event.
             */
            Value value;

            bool operator<(const Event& other) const
            {
                return timepoint < other.timepoint;
            }
        };

      private:
        std::multiset<Event> _events;

      public:
        /**
         * Default constructor for the EventSequence class.
         * Initializes an empty EventSequence object.
         *
         * @return A default-constructed EventSequence instance.
         */
        EventSequence() = default;

        /**
         * Retrieves a reference to the multiset containing all stored events.
         *
         * @return A reference to the multiset of Event objects.
         */
        std::multiset<Event>& getEvents()
        {
            return _events;
        }

        /**
         * Retrieves a reference to the multiset containing all stored events.
         *
         * @return A reference to the multiset of Event objects.
         */
        const std::multiset<Event>& getEvents() const
        {
            return _events;
        }

        /**
         * @return The timepoint of the first event in this sequence.
         */
        std::chrono::nanoseconds getStartTime() const
        {
            if (_events.empty()) {
                return std::chrono::nanoseconds::zero();
            }
            return _events.begin()->timepoint;
        }

        /**
         * @return The timepoint of the last event in this sequence.
         */
        std::chrono::nanoseconds getEndTime() const
        {
            if (_events.empty()) {
                return std::chrono::nanoseconds::zero();
            }
            return _events.rbegin()->timepoint;
        }

        /**
         * Calculates the time difference between the first and last event of this sequence.
         *
         * @return The duration of the event sequence in nanoseconds.
         */
        std::chrono::nanoseconds getDuration() const
        {
            return getEndTime() - getStartTime();
        }

        /**
         * Retrieves a range of events between the specified start and end times.
         *
         * The method determines the range of events within the specified time interval
         * [start, end). The times are converted to a common unit (nanoseconds) for comparison purposes.
         * This function returns a subrange which iterates the events that fall within the time interval.
         *
         * @param start The start time of the range as a duration.
         * @param end The end time of the range as a duration.
         * @return A subrange of the events between the given start and end time.
         */
        template<typename Rep1, typename Period1, typename Rep2, typename Period2>
        auto getRange(std::chrono::duration<Rep1, Period1> start, std::chrono::duration<Rep2, Period2> end) const
        {
            Event dummyStart{.timepoint = duration_cast<std::chrono::nanoseconds>(start)};
            Event dummyEnd{.timepoint = duration_cast<std::chrono::nanoseconds>(end)};

            auto first = _events.lower_bound(dummyStart);
            auto last = _events.lower_bound(dummyEnd);

            return std::ranges::subrange(first, last);
        }

        /**
         * Retrieves all events related to the element with the specified unique identifier.
         *
         * @param uid The unique identifier (UID) of the events to retrieve.
         * @return A view of the events in the sequence that match the given UID.
         */
        auto getEventsFor(UID uid) const
        {
            return _events | std::views::filter([uid](const Event& event) { return event.uid == uid; });
        }

        // Modifications

        /**
         * Reserves storage for the specified number of elements in the event container.
         *
         * @param size The number of elements to allocate storage for in advance.
         */
        void reserve(size_t size)
        {
            _events.reserve(size);
        }

        /**
         * Adds an event to the sequence.
         *
         * @param uid The unique identifier of the event.
         * @param duration The timepoint of the event relative to the start of the sequence.
         * @param value The value or payload of the event.
         */
        template<typename Rep, typename Period>
        void addEvent(UID uid, std::chrono::duration<Rep, Period> duration, Value value)
        {
            _events.insert({uid, duration_cast<std::chrono::nanoseconds>(duration), std::move(value)});
            incrementVersion();
        }
    };

} // namespace mindset

#endif // EVENTSEQUENCE_H
