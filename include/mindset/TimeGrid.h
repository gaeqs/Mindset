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

#ifndef TIMEGRID_H
#define TIMEGRID_H

#include <chrono>
#include <ranges>

#include <mindset/UID.h>
#include <mindset/Versioned.h>

namespace mindset
{

    /**
     * A data structure for managing time-stepped data associated with unique identifiers,
     * ideal for timelines where all the elements have a value each timestep.
     *
     * If you need a data structure that stores dispersed temporal events, use EventSequence.
     *
     * This structure defines two concepts: timestep and timeline.
     * - Timestep: collection of values for all elements at a given time.
     * - Timestep: collections of values for a element across the time.
     */
    template<typename Value>
    class TimeGrid : public Versioned
    {
        std::chrono::nanoseconds _delta;
        std::vector<UID> _uids;
        std::vector<std::vector<Value>> _data;

      public:
        template<typename Rep, typename Period>
        /**
         * Constructs a TimeGrid object with a specified time step (delta).
         *
         * The constructor takes a duration value of any type and converts it to nanoseconds.
         *
         * @tparam Rep The representation type of the duration.
         * @tparam Period The ratio of the duration's tick period.
         * @param delta The time duration that will be used as the time step
         *              for the grid, which is internally converted to nanoseconds.
         */
        explicit TimeGrid(std::chrono::duration<Rep, Period> delta) :
            _delta(std::chrono::duration_cast<std::chrono::nanoseconds>(delta))
        {
        }

        /**
         * @brief Returns the time step interval of the TimeGrid.
         *
         * This function provides the duration between consecutive time steps in
         * nanoseconds as defined when the TimeGrid was constructed.
         *
         * @return The time step interval as a std::chrono::nanoseconds object.
         */
        std::chrono::nanoseconds getDelta() const
        {
            return _delta;
        }

        /**
         * Retrieves the list of unique identifiers (UIDs).
         *
         * @return A constant reference to a vector containing the UIDs.
         */
        const std::vector<UID>& getUIDIndices() const
        {
            return _uids;
        }

        /**
         * Retrieves the total number of timesteps stored in the TimeGrid.
         *
         * @return The number of timesteps.
         */
        size_t getTimestepsAmount() const
        {
            return _data.size();
        }

        /**
         * Retrieves the dimensions of the grid.
         *
         * The method returns a pair of size_t values, representing the dimensions of
         * the grid. The first element corresponds to the number of UIDs,
         * and the second element corresponds to the number of timesteps.
         *
         * @return A pair consisting of the size of `_uids` and `_data`, in that order.
         */
        std::pair<size_t, size_t> getDimensions() const
        {
            return {_uids.size(), _data.size()};
        }

        /**
         * Retrieves a reference to the underlying 2D data structure representing
         * the timesteps and their associated values.
         *
         * @return A reference to the 2D vector containing the data.
         */
        std::vector<std::vector<Value>>& getData()
        {
            return _data;
        }

        /**
         * Retrieves a reference to the underlying 2D data structure representing
         * the timesteps and their associated values.
         *
         * @return A reference to the 2D vector containing the data.
         */
        const std::vector<std::vector<Value>>& getData() const
        {
            return _data;
        }

        /**
         * Finds the index of a given UID in the internal UID vector.
         * If the UID does not exist in the vector, an empty `std::optional` is returned.
         *
         * @param uid The UID to be searched for in the `_uids` vector.
         * @return An `std::optional` containing the index of the UID if found,
         *         or an empty `std::optional` if the UID is not present.
         */
        std::optional<size_t> findUIDIndex(UID uid) const
        {
            auto it = std::ranges::find(_uids, uid);
            if (it == _uids.end()) {
                return {};
            }
            return std::distance(_uids.begin(), it);
        }

        /**
         * Retrieves a pointer to a vector of values representing a specific
         * timestep at the given index. If the index is out of bounds, an
         * empty optional is returned.
         *
         * You can use the vector given by 'getUIDIndices' to fetch the UID
         * of each element inside this vector.
         *
         * @param index The index of the timestep to retrieve.
         * @return A std::optional containing a pointer to the vector of values
         *         for the specified timestep. If the index is invalid, an
         *         empty optional is returned.
         */
        std::optional<std::vector<Value>*> getTimestep(size_t index)
        {
            if (index >= _data.size()) {
                return {};
            }
            return &_data[index];
        }

        /**
         * Retrieves a pointer to a vector of values representing a specific
         * timestep at the given index. If the index is out of bounds, an
         * empty optional is returned.
         *
         * You can use the vector given by 'getUIDIndices' to fetch the UID
         * of each element inside this vector.
         *
         * @param index The index of the timestep to retrieve.
         * @return A std::optional containing a pointer to the vector of values
         *         for the specified timestep. If the index is invalid, an
         *         empty optional is returned.
         */
        std::optional<const std::vector<Value>*> getTimestep(size_t index) const
        {
            if (index >= _data.size()) {
                return {};
            }
            return &_data[index];
        }

        /**
         * Retrieves the closest timestep data corresponding to the specified time duration,
         * using a truncation policy without clamping.
         *
         * You can use the vector given by 'getUIDIndices' to fetch the UID
         * of each element inside this vector.
         *
         * @param time The time duration for which the closest timestep data is requested.
         * @return An optional containing a pointer to a const vector of Value representing the data
         *         at the closest timestep. If the index is out of range, an empty optional is returned.
         */
        template<typename Rep, typename Period>
        std::optional<const std::vector<Value>*> getClosestTimestep(std::chrono::duration<Rep, Period> time) const
        {
            auto castedTime = std::chrono::duration_cast<decltype(_delta)>(time);
            size_t index = castedTime / _delta;
            return getTimestep(index);
        }

        /**
         * Retrieves the closest timestep data corresponding to the specified time duration,
         * using a truncation policy without clamping.
         *
         * You can use the vector given by 'getUIDIndices' to fetch the UID
         * of each element inside this vector.
         *
         * @param time The time duration for which the closest timestep data is requested.
         * @return An optional containing a pointer to a const vector of Value representing the data
         *         at the closest timestep. If the index is out of range, an empty optional is returned.
         */
        template<typename Rep, typename Period>
        std::optional<std::vector<Value>*> getClosestTimestep(std::chrono::duration<Rep, Period> time)
        {
            auto castedTime = std::chrono::duration_cast<decltype(_delta)>(time);
            size_t index = castedTime / _delta;
            return getTimestep(index);
        }

        /**
         * Retrieves a timeline corresponding to the specified UID.
         * If the UID is not found, it returns an empty view.
         *
         * @param uid The unique identifier used to locate the corresponding timeline.
         * @return A view of the timeline corresponding to the UID. If the UID is not found, it returns an empty view.
         */
        auto getTimeline(UID uid)
        {
            auto index = findUIDIndex(uid);
            if (!index.has_value()) {
                return std::span<const std::vector<Value>>{} |
                       std::views::transform([](const std::vector<Value>& step) -> const Value& { return step[0]; });
            }
            return _data | std::views::transform(
                               [i = *index](const std::vector<Value>& step) -> const Value& { return step[i]; });
        }

        /**
         * Retrieves a timeline corresponding to the specified UID.
         * If the UID is not found, it returns an empty view.
         *
         * @param uid The unique identifier used to locate the corresponding timeline.
         * @return A view of the timeline corresponding to the UID. If the UID is not found, it returns an empty view.
         */
        auto getTimeline(UID uid) const
        {
            auto index = findUIDIndex(uid);
            if (!index.has_value()) {
                return std::span<const std::vector<Value>>{} |
                       std::views::transform([](const std::vector<Value>& step) -> const Value& { return step[0]; });
            }
            return _data | std::views::transform(
                               [i = *index](const std::vector<Value>& step) -> const Value& { return step[i]; });
        }

        // Modifications

        /**
         * Defines a set of unique identifiers (UIDs) and resizes the internal data
         * structure to match the number of UIDs provided.
         *
         * @param uids A vector of UIDs to define. Each UID represents a unique identifier
         *             for an element in the data structure. The size of the data is updated
         *             accordingly.
         */
        void defineUIDs(const std::vector<UID>& uids)
        {
            _uids = uids;
            for (auto& data : _data) {
                data.resize(_uids.size(), Value());
            }

            incrementVersion();
        }

        /**
         * Adds a new timestep to the TimeGrid. The provided timestep vector
         * is resized to match the number of UIDs, ensuring that all UIDs have
         * an associated value in the timestep. The resized vector is then stored
         * in the TimeGrid's internal data structure.
         *
         * @param timestep A vector of values representing a single timestep. The
         *                 vector will be resized to match the size of the UID list.
         */
        void addTimestep(std::vector<Value> timestep)
        {
            timestep.resize(_uids.size(), Value());
            _data.push_back(std::move(timestep));

            incrementVersion();
        }

        /**
         * Adds a timeline to the grid associated with a specific UID. If the UID is not present,
         * it will be added to the set of UIDs and a new timeline will be created. If the UID
         * already exists, the timeline is updated with provided values. Missing values in
         * the timeline will be filled with default-initialized instances of the `Value` type.
         *
         * @param uid The unique identifier for the timeline to be added or updated.
         * @param timeline A vector containing the timeline data to be added or updated.
         */
        void addTimeline(UID uid, const std::vector<Value>& timeline)
        {
            auto indexOptional = findUIDIndex(uid);
            size_t index;
            if (!indexOptional.has_value()) {
                index = _uids.size();
                _uids.push_back(uid);
            } else {
                index = indexOptional.value();
            }

            size_t oldRows = _data.size();
            size_t newRows = std::max(timeline.size(), oldRows);

            for (size_t i = 0; i < oldRows; ++i) {
                auto& row = _data[i];
                row.resize(_uids.size());
                if (i < timeline.size()) {
                    row[index] = timeline[i];
                } else {
                    row[index] = Value();
                }
            }

            for (size_t i = oldRows; i < newRows; ++i) {
                std::vector<Value> row(_uids.size()); // default‑initialized
                row[index] = timeline[i];
                _data.push_back(std::move(row));
            }

            incrementVersion();
        }
    };

} // namespace mindset

#endif // TIMEGRID_H
