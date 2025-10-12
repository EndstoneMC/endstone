// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <type_traits>
#include <variant>

namespace Details {
template <typename T>
class ValueOrRef {
public:
    ValueOrRef(std::reference_wrapper<T> ref) : is_pointer_(true), variant_(&ref.get()) {}

    ~ValueOrRef()
    {
        if (!is_pointer_) {
            variant_.value.~T();
        }
    }

    ValueOrRef(const ValueOrRef &other) = delete;

    ValueOrRef(ValueOrRef &&other) noexcept
        : is_pointer_(other.is_pointer_),
          variant_(other.is_pointer_ ? Variant(other.variant_.pointer) : Variant(std::move(other.variant_.value)))
    {
    }

    T &value() noexcept
    {
        return is_pointer_ ? *variant_.pointer : variant_.value;
    }

    const T &value() const noexcept
    {
        return is_pointer_ ? *variant_.pointer : variant_.value;
    }

private:
    ValueOrRef(T value) : is_pointer_(false), variant_(std::move(value)) {}

    union Variant {
        Variant() : pointer(nullptr) {}
        Variant(T *ptr) : pointer(ptr) {}
        Variant(T &&val) : value(std::move(val)) {}
        ~Variant() {}
        T *pointer;
        T value;
    };
    Variant variant_;
    const bool is_pointer_;
};
}  // namespace Details

template <typename... Xs>
class EventVariantImpl {
    using variant_t = std::variant<Details::ValueOrRef<Xs>...>;

public:
    template <typename T>
    EventVariantImpl(std::reference_wrapper<T> event) : variant_{std::in_place_type<Details::ValueOrRef<T>>, event}
    {
    }

    EventVariantImpl(const EventVariantImpl &other)
        : variant_(const_cast<EventVariantImpl &>(other).visit([](auto &&arg) -> variant_t {
              using T = std::remove_reference_t<decltype(arg.value())>;
              if constexpr (std::is_const_v<T>) {
                  return variant_t{std::in_place_type<Details::ValueOrRef<T>>, std::cref(arg.value())};
              }
              else {
                  return variant_t{std::in_place_type<Details::ValueOrRef<T>>, std::ref(arg.value())};
              }
          }))
    {
    }

    template <typename F>
    auto visit(F &&visitor)
    {
        return std::visit(std::forward<F>(visitor), variant_);
    }

    template <typename F>
    auto visit(F &&visitor) const
    {
        return std::visit(std::forward<F>(visitor), variant_);
    }

private:
    variant_t variant_;
};

template <typename... Events>
using ConstEventVariant = EventVariantImpl<std::add_const_t<Events>...>;

template <typename... Events>
using MutableEventVariant = EventVariantImpl<Events...>;

template <typename EventVariant>
class EventRef {
    static_assert(std::is_copy_constructible_v<EventVariant>, "EventVariant must be copy constructible");

public:
    template <typename Event>
    EventRef(Event &event)
        requires(!std::same_as<std::decay_t<Event>, EventRef> && std::is_const_v<Event>)
        : variant_(std::cref(event))
    {
    }

    template <typename Event>
    EventRef(Event &event)
        requires(!std::same_as<std::decay_t<Event>, EventRef> && !std::is_const_v<Event>)
        : variant_(std::ref(event))
    {
    }

    EventRef(const EventRef &other) = default;

    EventVariant &get()
    {
        return variant_;
    }

    EventVariant const &get() const
    {
        return variant_;
    }

private:
    EventVariant variant_;
};

namespace Details {
template <typename...>
struct TemplateArgumentCat;

template <typename... Xs>
struct TemplateArgumentCat<EventVariantImpl<Xs...>> {
    using type = EventVariantImpl<Xs...>;
};

template <typename... Xs, typename... Ys, typename... Rest>
struct TemplateArgumentCat<EventVariantImpl<Xs...>, EventVariantImpl<Ys...>, Rest...> {
    using type = TemplateArgumentCat<EventVariantImpl<Xs..., Ys...>, Rest...>::type;
};
}  // namespace Details

template <typename... Events>
using NotificationEventVariant = Details::TemplateArgumentCat<Events...>::type;
