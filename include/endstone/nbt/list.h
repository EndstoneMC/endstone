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

#include <algorithm>
#include <cstddef>
#include <stdexcept>
#include <type_traits>
#include <utility>
#include <vector>

#include "endstone/nbt/type.h"

namespace endstone {
namespace nbt {
class Tag;
}

class ListTag : public nbt::TagBase {
public:
    using value_type = nbt::Tag;
    using size_type = std::size_t;
    using container_type = std::vector<value_type>;
    using iterator = container_type::iterator;
    using const_iterator = container_type::const_iterator;

    // ---- ctors ----
    ListTag() = default;
    template <typename T>
        requires(!std::is_same_v<std::remove_cvref_t<T>, nbt::Tag>)
    ListTag(std::initializer_list<T> init);

    // ---- capacity ----
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] size_type size() const noexcept;

    // ---- element typing ----
    [[nodiscard]] nbt::Type type() const noexcept;

    // ---- element access ----
    value_type &at(size_type i);
    [[nodiscard]] const value_type &at(size_type i) const;
    value_type &operator[](size_type i);
    const value_type &operator[](size_type i) const;

    // ---- modifiers ----
    void clear() noexcept;
    void push_back(const value_type &v);
    void push_back(value_type &&v);

    template <class... Args>
    value_type &emplace_back(Args &&...args);
    iterator erase(const_iterator pos);
    iterator erase(const_iterator first, const_iterator last);

    // ---- iteration ----
    iterator begin() noexcept;
    iterator end() noexcept;
    [[nodiscard]] const_iterator begin() const noexcept;
    [[nodiscard]] const_iterator end() const noexcept;
    [[nodiscard]] const_iterator cbegin() const noexcept;
    [[nodiscard]] const_iterator cend() const noexcept;

    // ---- equality ----
    friend bool operator==(const ListTag &a, const ListTag &b) noexcept;
    friend bool operator!=(const ListTag &a, const ListTag &b) noexcept;

private:
    nbt::Type type_{nbt::Type::End};
    container_type elements_;
};

}  // namespace endstone
