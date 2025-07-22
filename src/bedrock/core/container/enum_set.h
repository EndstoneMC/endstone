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

#include "bedrock/platform/brstd/bitset.h"

namespace Bedrock {
template <typename TEnum, TEnum SizeEnum>
class EnumSet {
    using size_type = size_t;
    static constexpr size_type MaxSize = static_cast<size_type>(SizeEnum);
    using container_type = brstd::bitset<5UL, unsigned char>;
    using value_type = TEnum;

public:
    class iterator {
        using reference = TEnum;

    public:
        iterator();
        iterator::reference operator*() const;
        iterator &operator++();
        iterator operator++(int);

    private:
        iterator(const EnumSet &, const size_type);
        const EnumSet *set_;
        size_type index_;
    };
    static_assert(sizeof(iterator) == 16);

private:
    using const_iterator = iterator;

public:
    EnumSet();
    EnumSet(std::initializer_list<TEnum>);
    bool empty() const;
    size_type size() const;
    size_type max_size() const;
    void clear();
    std::pair<iterator, bool> insert(const value_type);
    void insert(std::initializer_list<TEnum>);
    bool erase(const value_type);
    iterator erase(const const_iterator);
    const_iterator find(const value_type) const;
    bool contains(const value_type) const;
    void merge(const EnumSet &);
    void intersect(const EnumSet &);
    void difference(const EnumSet &);
    bool includes(const EnumSet &) const;
    bool overlaps(const EnumSet &) const;
    bool disjoint(const EnumSet &) const;
    iterator begin();
    const_iterator begin() const;
    const_iterator cbegin() const;
    iterator end();
    const_iterator end() const;
    const_iterator cend() const;
    void swap(EnumSet &);
    bool operator==(const EnumSet &) const;

private:
    container_type container_;
};
}  // namespace Bedrock
