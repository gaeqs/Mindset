//
// Created by gaeqs on 09/03/2025.
//

#ifndef IDENTIFIABLE_H
#define IDENTIFIABLE_H

#include <functional>

#include <mnemea/UID.h>

namespace mnemea {
    class Identifiable {
        UID _id;

    public:
        explicit Identifiable(UID id);

        [[nodiscard]] UID getUID() const;

        bool operator==(const Identifiable& other) const;

        bool operator!=(const Identifiable& other) const;
    };
}

template<typename T>
    requires std::is_base_of_v<mnemea::Identifiable, T>
struct std::hash<T> {
    size_t operator()(const T& s) const noexcept {
        return std::hash<mnemea::UID>()(s.getUID());
    }
};

#endif //IDENTIFIABLE_H
