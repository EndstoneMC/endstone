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
#include <utility>
#include <vector>

#include "endstone/nbt/type.h"

namespace endstone::nbt {
template <typename T>
class ArrayTag : public TagBase {
public:
    using value_type = T;
    using size_type = std::size_t;
    using storage_type = std::vector<value_type>;
    using iterator = storage_type::iterator;
    using const_iterator = storage_type::const_iterator;
    using difference_type = storage_type::difference_type;

    // Constructors
    ArrayTag() = default;
    explicit ArrayTag(storage_type v) : v_(std::move(v)) {}
    template <class It>
    ArrayTag(It first, It last) : v_(first, last)
    {
    }
    ArrayTag(std::initializer_list<value_type> init) : v_(init) {}  // {1,2,3}

    // Capacity
    bool empty() const noexcept { return v_.empty(); }
    size_type size() const noexcept { return v_.size(); }
    void clear() noexcept { v_.clear(); }

    // Element access
    value_type &at(size_type i)
    {
        if (i >= v_.size()) {
            throw std::out_of_range("ArrayTag::at");
        }
        return v_[i];
    }
    const value_type &at(size_type i) const
    {
        if (i >= v_.size()) {
            throw std::out_of_range("ArrayTag::at");
        }
        return v_[i];
    }
    value_type &operator[](size_type i) { return v_[i]; }
    const value_type &operator[](size_type i) const { return v_[i]; }
    value_type *data() noexcept { return v_.data(); }
    const value_type *data() const noexcept { return v_.data(); }

    // Modifiers
    void push_back(value_type v) { v_.push_back(v); }
    template <class It>
    void assign(It first, It last)
    {
        v_.assign(first, last);
    }
    iterator insert(const_iterator pos, value_type v) { return v_.insert(pos, v); }
    iterator erase(const_iterator pos) { return v_.erase(pos); }

    // Iteration
    iterator begin() noexcept { return v_.begin(); }
    iterator end() noexcept { return v_.end(); }
    const_iterator begin() const noexcept { return v_.begin(); }
    const_iterator end() const noexcept { return v_.end(); }
    const_iterator cbegin() const noexcept { return v_.cbegin(); }
    const_iterator cend() const noexcept { return v_.cend(); }

    // Equality
    friend bool operator==(const ArrayTag &a, const ArrayTag &b) noexcept { return a.v_ == b.v_; }
    friend bool operator!=(const ArrayTag &a, const ArrayTag &b) noexcept { return !(a == b); }
    friend bool operator==(const ArrayTag &a, const storage_type &b) noexcept { return a.v_ == b; }
    friend bool operator==(const storage_type &a, const ArrayTag &b) noexcept { return a == b.v_; }
    friend bool operator!=(const ArrayTag &a, const storage_type &b) noexcept { return !(a == b); }
    friend bool operator!=(const storage_type &a, const ArrayTag &b) noexcept { return !(a == b); }

private:
    storage_type v_;
};
}  // namespace endstone::nbt
