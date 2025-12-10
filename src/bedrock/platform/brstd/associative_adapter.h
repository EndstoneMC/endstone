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

namespace brstd {

struct no_value_t {};

struct no_mapped_container_t {};

template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
class associative_adapter {
    template <typename, typename, typename>
    class iterator_impl;

public:
    static constexpr bool no_mapped_container = std::is_same_v<MappedContainer, no_mapped_container_t>;
    using key_type = Key;
    using key_compare = Compare;
    using size_type = KeyContainer::size_type;
    using difference_type = KeyContainer::difference_type;
    using key_container_type = KeyContainer;
    using mapped_container_type = MappedContainer;

private:
    template <typename Derived, typename KeysIterator, typename ValuesIterator>
    class iterator_impl_base {
    public:
        iterator_impl_base() = default;
        iterator_impl_base(KeysIterator keys_it, ValuesIterator values_it) : keys_it_(keys_it), values_it_(values_it) {}
        iterator_impl_base(const iterator_impl_base &) = default;
        iterator_impl_base(iterator_impl_base &&) noexcept = default;
        iterator_impl_base &operator=(const iterator_impl_base &) = default;
        iterator_impl_base &operator=(iterator_impl_base &&) noexcept = default;
        Derived &operator++()
        {
            ++keys_it_;
            ++values_it_;
            return static_cast<Derived &>(*this);
        }

        Derived operator++(int)
        {
            auto copy = static_cast<Derived &>(*this);
            ++(*this);
            return copy;
        }

        Derived &operator--()
        {
            --keys_it_;
            --values_it_;
            return static_cast<Derived &>(*this);
        }

        Derived operator--(int)
        {
            auto copy = static_cast<Derived &>(*this);
            --(*this);
            return copy;
        }

        Derived &operator+=(difference_type n)
        {
            keys_it_ += n;
            values_it_ += n;
            return static_cast<Derived &>(*this);
        }

        Derived &operator-=(difference_type n)
        {
            keys_it_ -= n;
            values_it_ -= n;
            return static_cast<Derived &>(*this);
        }

        Derived operator+(difference_type n) const
        {
            return Derived(keys_it_ + n, values_it_ + n);
        }

        Derived operator-(difference_type n) const
        {
            return Derived(keys_it_ - n, values_it_ - n);
        }

        bool operator==(const iterator_impl_base &other) const
        {
            return keys_it_ == other._keys_it;
        }

        std::strong_ordering operator<=>(iterator_impl_base &other) const
        {
            return keys_it_ <=> other._keys_it;
        }

    protected:
        KeysIterator keys_it_;
        ValuesIterator values_it_;
    };

public:
    struct containers {
        key_container_type keys;       // +0
        mapped_container_type values;  // +24
    };

    auto begin() noexcept
    {
        return containers_.keys.begin();
    }

    auto begin() const noexcept
    {
        return containers_.keys.begin();
    }

    auto end() noexcept
    {
        return containers_.keys.end();
    }

    auto end() const noexcept
    {
        return containers_.keys.end();
    }

private:
    containers containers_;  // +0
    key_compare compare_;    // +32
};
};  // namespace brstd
