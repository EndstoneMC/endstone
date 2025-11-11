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
#include <stdexcept>
#include <vector>

#include "endstone/nbt/type.h"

namespace endstone {
namespace nbt {
class Tag;
}

class CompoundTag {
public:
    using key_type = std::string;
    using value_type = nbt::Tag;
    using map_type = std::map<key_type, value_type>;
    using size_type = std::size_t;
    using iterator = map_type::iterator;
    using const_iterator = map_type::const_iterator;

    // Constructors
    CompoundTag();
    CompoundTag(std::initializer_list<std::pair<std::string, nbt::Tag>> init);
    template <class It>
    CompoundTag(It first, It last);

    // Capacity
    bool empty() const noexcept;
    size_type size() const noexcept;

    // Lookup / element access
    nbt::Tag &at(std::string_view key);
    const nbt::Tag &at(std::string_view key) const;
    nbt::Tag &operator[](const std::string &key);  // inserts End if absent
    nbt::Tag &operator[](std::string &&key);
    bool contains(std::string_view key) const noexcept;
    size_type count(std::string_view key) const noexcept;

    // Modifiers
    void clear() noexcept;
    std::pair<iterator, bool> insert(const std::pair<const std::string, nbt::Tag> &v);
    std::pair<iterator, bool> insert(std::pair<const std::string, nbt::Tag> &&v);
    template <class P>
    std::pair<iterator, bool> insert(P &&v);
    template <class... Args>
    std::pair<iterator, bool> try_emplace(std::string key, Args &&...ctor_args);
    template <class M>
    std::pair<iterator, bool> insert_or_assign(std::string key, M &&obj);
    iterator erase(const_iterator pos);
    size_type erase(std::string_view key);
    iterator erase(const_iterator first, const_iterator last);
    void swap(CompoundTag &other) noexcept;
    void merge(CompoundTag &source);
    void merge(CompoundTag &&source);

    // Iteration
    iterator begin() noexcept;
    iterator end() noexcept;
    const_iterator begin() const noexcept;
    const_iterator end() const noexcept;
    const_iterator cbegin() const noexcept;
    const_iterator cend() const noexcept;

    // Equality
    friend bool operator==(const CompoundTag &a, const CompoundTag &b) noexcept;
    friend bool operator!=(const CompoundTag &a, const CompoundTag &b) noexcept;

private:
    map_type entries_;
};
}  // namespace endstone
