//
// Created by gaeqs on 09/03/2025.
//

#ifndef IDENTIFIABLE_H
#define IDENTIFIABLE_H

#include <functional>

#include <mnemea/UID.h>


namespace mnemea
{
    /**
     * The Identifiable class provides a unique identifier to objects.
     */
    class Identifiable
    {
        UID _id;

      public:
        /**
         * Constructs an Identifiable object with a given UID.
         * @param id Unique identifier.
         */
        explicit Identifiable(UID id);

        /**
         * Returns the UID of the object.
         */
        [[nodiscard]] UID getUID() const;

        /**
         * Sets the UID of the object.
         * @param uid New UID to assign.
         */
        void setUID(UID uid);

        bool operator==(const Identifiable& other) const;

        bool operator!=(const Identifiable& other) const;
    };
} // namespace mnemea

template<typename T>
    requires std::is_base_of_v<mnemea::Identifiable, T>
struct std::hash<T>
{
    size_t operator()(const T& s) const noexcept
    {
        return std::hash<mnemea::UID>()(s.getUID());
    }
};

#endif //IDENTIFIABLE_H
