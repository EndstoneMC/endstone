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
    ValueOrRef(std::reference_wrapper<T> ref) : is_pointer_(true)
    {
        variant_.pointer = &ref.get();
    }

    ~ValueOrRef()
    {
        if (!is_pointer_) {
            variant_.value.~T();
        }
    }

    T &value() noexcept
    {
        return is_pointer_ ? *variant_.pointer : variant_.value;
    }

    T &value() const noexcept
    {
        return is_pointer_ ? *variant_.pointer : variant_.value;
    }

private:
    ValueOrRef(T value) : is_pointer_(false)
    {
        variant_.value = value;
    }

    union Variant {
        Variant() {}
        ~Variant() {}
        T *pointer;
        T value;
    } variant_;
    const bool is_pointer_;
};
}  // namespace Details

template <typename... Events>
struct EventVariantImpl {
    EventVariantImpl(EventVariantImpl const &) = default;
    EventVariantImpl(EventVariantImpl &&) = default;

    template <typename T>
    EventVariantImpl(std::reference_wrapper<T> event) : variant{std::in_place_type<Details::ValueOrRef<T>>, event}
    {
    }

    std::variant<Details::ValueOrRef<Events>...> variant;
};

template <typename... Events>
using ConstEventVariant = EventVariantImpl<std::add_const_t<Events>...>;

template <typename... Events>
using MutableEventVariant = EventVariantImpl<Events...>;

template <typename EventVariant>
class EventRef {
public:
    EventRef(EventRef const &) = delete;

    template <typename Event>
    EventRef(Event &event) : variant_(std::ref(event))
    {
    }

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
