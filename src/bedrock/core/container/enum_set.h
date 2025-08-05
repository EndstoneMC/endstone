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
public:
    using size_type = size_t;
    static constexpr size_type MaxSize = static_cast<size_type>(SizeEnum);
    using container_type = brstd::bitset<MaxSize, std::underlying_type_t<TEnum>>;
    using value_type = TEnum;

    class iterator {
        using reference = TEnum;

    public:
        iterator();
        iterator::reference operator*() const;
        iterator &operator++();
        iterator operator++(int);

    private:
        friend class EnumSet;
        iterator(const EnumSet &set, size_type index) : set_(&set), index_(index) {}
        const EnumSet *set_;
        size_type index_;
    };
    static_assert(sizeof(iterator) == 16);

    using const_iterator = iterator;

    EnumSet();
    EnumSet(std::initializer_list<value_type>);
    bool empty() const;
    size_type size() const;
    size_type max_size() const;
    void clear();

    constexpr std::pair<iterator, bool> insert(value_type value)
    {
        using U = std::underlying_type_t<TEnum>;
        auto idx = static_cast<U>(value);
        if (container_.test(idx)) {
            return {iterator(*this, idx), false};
        }
        container_.set(idx);
        return {iterator(*this, idx), true};
    }

    void insert(std::initializer_list<value_type>);
    bool erase(const value_type);
    iterator erase(const const_iterator);
    const_iterator find(const value_type) const;
    constexpr bool contains(const value_type value) const
    {
        using U = std::underlying_type_t<TEnum>;
        return container_.test(static_cast<U>(value));
    }
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
