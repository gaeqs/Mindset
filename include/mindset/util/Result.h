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

#ifndef MINDSET_RESULT_H
#define MINDSET_RESULT_H

#include <cstdint>
#include <functional>

namespace mindset
{

    /**
     * Represents the result of an operation, which may contain either a successful value or an error.
     * @tparam Ok The type of the result on success.
     * @tparam Error The type representing the error.
     */
    template<class Ok, class Error>
    class Result
    {
        void* _data;
        bool _valid;

      public:
        /**
         * Constructs a successful result.
         * @param ok The successful result value.
         */
        Result(const Ok& ok) :
            _valid(true)
        {
            _data = new uint8_t[sizeof(Ok)];
            std::construct_at<Ok, const Ok&>(static_cast<Ok*>(_data), ok);
        }

        /**
         * Constructs a successful result.
         * @param ok The successful result value.
         */
        Result(Ok&& ok) :
            _valid(true)
        {
            _data = new uint8_t[sizeof(Ok)];
            std::construct_at<Ok, Ok&&>(static_cast<Ok*>(_data), std::forward<Ok>(ok));
        }

        /**
         * Constructs an error result.
         * @param error The error value.
         */
        Result(const Error& error) :
            _valid(false)
        {
            _data = new uint8_t[sizeof(Error)];
            std::construct_at<Error, const Error&>(static_cast<Error*>(_data), error);
        }

        /**
         * Constructs an error result.
         * @param error The error value.
         */
        Result(Error&& error) :
            _valid(false)
        {
            _data = new uint8_t[sizeof(Error)];
            std::construct_at<Error, Error&&>(static_cast<Error*>(_data), std::forward<Error>(error));
        }

        ~Result()
        {
            if (_valid) {
                std::destroy_at<Ok>(static_cast<Ok*>(_data));
            } else {
                std::destroy_at<Error>(static_cast<Error*>(_data));
            }
            delete[] static_cast<uint8_t*>(_data);
        }

        /**
         * Checks if the result is successful.
         */
        [[nodiscard]] bool isOk() const
        {
            return _valid;
        }

        /**
         * Retrieves the result value.
         * @return Reference to the result value.
         */
        [[nodiscard]] Ok& getResult()
        {
            return *static_cast<Ok*>(_data);
        }

        /**
         * Retrieves the result value.
         * @return Reference to the result value.
         */
        [[nodiscard]] const Ok& getResult() const
        {
            return *static_cast<Ok*>(_data);
        }

        /**
         * Retrieves the error value.
         * @return Reference to the error value.
         */
        [[nodiscard]] Error& getError()
        {
            return *static_cast<Error*>(_data);
        }

        /**
         * Retrieves the error value.
         * @return Reference to the error value.
         */
        [[nodiscard]] const Error& getError() const
        {
            return *static_cast<Error*>(_data);
        }

        /**
         * Returns the result if successful, otherwise returns the provided fallback value.
         * @param other Fallback value.
         */
        [[nodiscard]] Ok& orElse(Ok& other)
        {
            return _valid ? getResult() : other;
        }

        /**
         * Returns the result if successful, otherwise returns the provided fallback value.
         * @param other Fallback value.
         */
        [[nodiscard]] const Ok& orElse(Ok& other) const
        {
            return _valid ? getResult() : other;
        }

        /**
         * Returns the result if successful, otherwise computes it using the provided function.
         * @param provider Function providing the fallback value.
         */
        [[nodiscard]] Ok orElseGet(std::function<Ok()> provider) const
        {
            return _valid ? getResult() : provider();
        }

        /**
         * Maps the result to another type if successful.
         * @tparam OOk Type of the mapped result.
         * @param mapper Function to transform the result.
         */
        template<class OOk>
        [[nodiscard]] Result<OOk, Error> map(std::function<OOk(Ok)> mapper) const
        {
            // Don't use a ternary operator.
            // The return value is implicitly transformed
            // into a Result using different constructors.
            // Using a ternary operator disallows that.
            if (_valid) {
                return mapper(getResult());
            }
            return getError();
        }

        /**
         * Maps the error to another type if an error occurred.
         * @tparam OError Type of the mapped error.
         * @param mapper Function to transform the error.
         */
        template<class OError>
        [[nodiscard]] Result<OError, Error> mapError(std::function<OError(Error)> mapper) const
        {
            // Don't use a ternary operator.
            // The return value is implicitly transformed
            // into a Result using different constructors.
            // Using a ternary operator disallows that.
            if (_valid) {
                return getResult();
            }
            return mapper(getError());
        }
    };
} // namespace mindset

#endif //MINDSET_RESULT_H
