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

#include <cstddef>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>

#include <pybind11/pybind11.h>

namespace endstone::python {

template <std::size_t N>
struct FixedString {
    char value[N]{};
    static constexpr std::size_t size = N - 1;

    constexpr FixedString(const char (&s)[N])
    {
        for (std::size_t i = 0; i < N; ++i) {
            value[i] = s[i];
        }
    }
};

template <FixedString... Vs>
class Literal {
public:
    Literal() = default;

    // NOLINTNEXTLINE(google-explicit-constructor)
    Literal(std::string s) : value_(std::move(s))
    {
        if (!is_valid(value_)) {
            throw std::invalid_argument("Invalid literal value: " + value_);
        }
    }

    [[nodiscard]] const std::string &value() const { return value_; }

    // NOLINTNEXTLINE(google-explicit-constructor)
    operator const std::string &() const { return value_; }

    static bool is_valid(const std::string &s) { return ((s == std::string_view(Vs.value, Vs.size)) || ...); }

private:
    std::string value_;
};

namespace detail {

template <FixedString S, std::size_t... Is>
constexpr auto fs_to_descr(std::index_sequence<Is...>)
{
    return pybind11::detail::descr<S.size>(S.value[Is]...);
}

template <FixedString S>
constexpr auto fs_to_descr()
{
    return fs_to_descr<S>(std::make_index_sequence<S.size>{});
}

template <FixedString V>
constexpr auto quoted_literal()
{
    return pybind11::detail::const_name("'") + fs_to_descr<V>() + pybind11::detail::const_name("'");
}

template <FixedString... Vs>
constexpr auto literal_type_name()
{
    return pybind11::detail::const_name("typing.Literal[") + pybind11::detail::concat(quoted_literal<Vs>()...) +
           pybind11::detail::const_name("]");
}

}  // namespace detail
}  // namespace endstone::python

namespace pybind11::detail {

template <endstone::python::FixedString... Vs>
class type_caster<endstone::python::Literal<Vs...>> {
public:
    bool load(handle src, bool)
    {
        if (!src || !PyUnicode_Check(src.ptr())) {
            return false;
        }
        value = src.cast<std::string>();
        if (!endstone::python::Literal<Vs...>::is_valid(value)) {
            return false;
        }
        return true;
    }

    static handle cast(const endstone::python::Literal<Vs...> &src, return_value_policy policy, handle parent)
    {
        return make_caster<std::string>::cast(src.value(), policy, parent);
    }

    PYBIND11_TYPE_CASTER(endstone::python::Literal<Vs...>, endstone::python::detail::literal_type_name<Vs...>());
};

}  // namespace pybind11::detail
