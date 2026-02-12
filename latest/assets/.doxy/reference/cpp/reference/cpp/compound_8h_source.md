

# File compound.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**nbt**](dir_777e494cd9d9b9aeedcd0556d83eeb02.md) **>** [**compound.h**](compound_8h.md)

[Go to the documentation of this file](compound_8h.md)


```C++
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
#include <map>
#include <stdexcept>

#include "endstone/nbt/type.h"

namespace endstone {
namespace nbt {
class Tag;
}

class CompoundTag : public nbt::TagBase {
public:
    using key_type = std::string;
    using value_type = nbt::Tag;
    using map_type = std::map<key_type, value_type>;
    using size_type = std::size_t;
    using iterator = map_type::iterator;
    using const_iterator = map_type::const_iterator;

    // Constructors
    CompoundTag() = default;
    CompoundTag(std::initializer_list<std::pair<key_type, value_type>> init);

    // Capacity
    [[nodiscard]] bool empty() const noexcept;
    [[nodiscard]] size_type size() const noexcept;

    // Lookup / element access
    value_type &at(const key_type &key);
    [[nodiscard]] const value_type &at(key_type key) const;
    value_type &operator[](const key_type &key);
    [[nodiscard]] bool contains(const key_type &key) const noexcept;

    // Modifiers
    void clear() noexcept;
    template <class P>
    std::pair<iterator, bool> insert(P &&v);
    template <class... Args>
    std::pair<iterator, bool> emplace(Args &&...args);
    template <class... Args>
    std::pair<iterator, bool> try_emplace(const key_type &key, Args &&...args);
    template <class M>
    std::pair<iterator, bool> insert_or_assign(const key_type &key, M &&obj);
    iterator erase(const_iterator pos);
    size_type erase(const key_type &key);
    iterator erase(const_iterator first, const_iterator last);
    void swap(CompoundTag &other) noexcept;
    void merge(CompoundTag &source);
    void merge(CompoundTag &&source);

    // Iteration
    iterator begin() noexcept;
    iterator end() noexcept;
    [[nodiscard]] const_iterator begin() const noexcept;
    [[nodiscard]] const_iterator end() const noexcept;
    [[nodiscard]] const_iterator cbegin() const noexcept;
    [[nodiscard]] const_iterator cend() const noexcept;

    // Equality
    friend bool operator==(const CompoundTag &a, const CompoundTag &b) noexcept;
    friend bool operator!=(const CompoundTag &a, const CompoundTag &b) noexcept;

private:
    map_type entries_;
};
}  // namespace endstone
```


