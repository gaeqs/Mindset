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


#ifndef CONTEXTUALIZED_H
#define CONTEXTUALIZED_H

#include <ranges>

#include <mindset/DefaultProperties.h>
#include <mindset/PropertyHolder.h>
#include <mindset/Context.h>
#include <mindset/util/NeuronTransform.h>

#include <rush/rush.h>

namespace mindset
{

    template<typename T>
    concept RangeAdaptorClosure = requires(T t) {
        { t(std::views::iota(0, 1)) } -> std::ranges::range;
    };

    enum class ContextualizedError
    {
        HOLDER_IS_CONST,
        CONTEXT_IS_CONST,
    };

    template<typename Holder, typename C>
        requires(std::is_base_of_v<PropertyHolder, Holder> && std::is_base_of_v<Context, C>)
    class Contextualized
    {
        Holder* _holder;
        C* _context;

        std::optional<UID> fetchPropertyId(const std::string& name, bool defineIfNotFound) const
        {
            if (std::is_const_v<C> || !defineIfNotFound) {
                return _context->getProperties().getPropertyUID(name);
            }
            return _context->getProperties().defineProperty(name);
        }

      public:
        Contextualized(Holder* holder, C* context) :
            _holder(holder),
            _context(context)
        {
        }

        Holder* getRaw() const
        {
            return _holder;
        }

        [[nodiscard]] std::optional<ContextualizedError> setProperty(const std::string& name, std::any value) const
        {
            if constexpr (std::is_const_v<Holder>) {
                return ContextualizedError::HOLDER_IS_CONST;
            }

            if (auto propId = fetchPropertyId(name, true); propId.has_value()) {
                _holder->setProperty(propId.value(), value);
                return {};
            }
            return ContextualizedError::CONTEXT_IS_CONST;
        }

        [[nodiscard]] std::optional<std::any> getPropertyAsAny(const std::string& name) const
        {
            if constexpr (std::is_const_v<Holder>) {
                return ContextualizedError::HOLDER_IS_CONST;
            }

            if (auto propId = fetchPropertyId(name, false); propId.has_value()) {
                return _holder->getPropertyAsAny(propId.value());
            }
            return {};
        }

        template<typename T>
        [[nodiscard]] std::optional<T> getProperty(const std::string& name) const
        {
            auto optional = getPropertyAsAny(name);
            if (!optional.has_value()) {
                return {};
            }
            T* v = std::any_cast<T>(&optional.value());
            if (v == nullptr) {
                return {};
            }
            return std::optional<T>(*v);
        }

        Holder* operator->() const
        {
            return _holder;
        }

        // Util methods

        template<typename T = UID>
        std::optional<T> getParent()
        {
            return getProperty<T>(PROPERTY_PARENT);
        }

        template<typename T = float>
        std::optional<T> getRadius()
        {
            return getProperty<T>(PROPERTY_RADIUS);
        }

        template<typename T = rush::Vec3f>
        std::optional<T> getPosition()
        {
            return getProperty<T>(PROPERTY_POSITION);
        }

        template<typename T = NeuriteType>
        std::optional<T> getNeuriteType()
        {
            return getProperty<T>(PROPERTY_NEURITE_TYPE);
        }

        template<typename T = NeuronTransform>
        std::optional<T> getTransform()
        {
            return getProperty<T>(PROPERTY_TRANSFORM);
        }

        template<typename T = UID>
        std::optional<T> getColumn()
        {
            return getProperty<T>(PROPERTY_COLUMN);
        }

        template<typename T = UID>
        std::optional<T> getMiniColumn()
        {
            return getProperty<T>(PROPERTY_MINI_COLUMN);
        }

        template<typename T = UID>
        std::optional<T> getLayer()
        {
            return getProperty<T>(PROPERTY_LAYER);
        }

        template<typename T = std::string>
        std::optional<T> getName()
        {
            return getProperty<T>(PROPERTY_NAME);
        }
    };

    template<typename Holder, typename C>
        requires(std::is_base_of_v<PropertyHolder, Holder> && std::is_base_of_v<Context, C>)
    Contextualized<Holder, C> operator|(Holder& holder, C& context)
    {
        return Contextualized<Holder, C>(&holder, &context);
    }

    template<std::ranges::range R, typename C>
        requires std::is_base_of_v<Context, C>
    auto operator|(R&& range, C& context)
    {
        return std::forward<R>(range) | std::views::transform([&context](auto&& element) {
                   using PtrType = std::remove_reference_t<decltype(element)>;
                   using ElemType = std::remove_pointer_t<PtrType>;
                   return Contextualized<ElemType, C>(element, &context);
               });
    }

    template<std::ranges::range R, typename C>
        requires std::is_base_of_v<Context, C>
    auto operator|(R&& range, C* context)
    {
        return std::forward<R>(range) | std::views::transform([context](auto&& element) {
                   using PtrType = std::remove_reference_t<decltype(element)>;
                   using ElemType = std::remove_pointer_t<PtrType>;
                   return Contextualized<ElemType, C>(element, context);
               });
    }
} // namespace mindset

#endif // CONTEXTUALIZED_H
