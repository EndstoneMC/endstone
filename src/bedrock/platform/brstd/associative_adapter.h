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

#ifdef _MSC_VER
#define NO_UNIQUE_ADDRESS [[msvc::no_unique_address]]
#else
#define NO_UNIQUE_ADDRESS [[no_unique_address]]
#endif

struct no_value_t {};

struct no_mapped_container_t {
    struct iterator {
        using difference_type = ptrdiff_t;
        std::strong_ordering operator<=>(const iterator &) const = default;
        no_value_t &operator*() const
        {
            static no_value_t value;
            return value;
        }
        iterator &operator++() { return *this; }
        iterator operator++(int) { return *this; }
        iterator &operator--() { return *this; }
        iterator operator--(int) { return *this; }
        iterator operator+(difference_type) const { return *this; }
        iterator operator-(difference_type) const { return *this; }
        iterator &operator+=(difference_type) { return *this; }
        iterator &operator-=(difference_type) { return *this; }
    };
    using const_iterator = iterator;
    std::strong_ordering operator<=>(const no_mapped_container_t &) const = default;
    [[nodiscard]] iterator begin() const { return iterator(); }
    [[nodiscard]] iterator end() const { return iterator(); }
    [[nodiscard]] iterator rbegin() const { return iterator(); }
    [[nodiscard]] iterator rend() const { return iterator(); }
    [[nodiscard]] iterator cbegin() const { return iterator(); }
    [[nodiscard]] iterator cend() const { return iterator(); }
    [[nodiscard]] iterator crbegin() const { return iterator(); }
    [[nodiscard]] iterator crend() const { return iterator(); }
};

template <typename Key, typename T, typename Compare, typename KeyContainer, typename MappedContainer>
class associative_adapter {
    template <typename KeysIterator, typename ValuesIterator, typename ValueType>
    class iterator_impl;

public:
    static constexpr bool no_mapped_container = std::is_same_v<MappedContainer, no_mapped_container_t>;
    template <bool NoMappedContainer>
    struct traits_t;
    template <>
    struct traits_t<true> {
        using mapped_type = Key;
        using value_type = Key;
    };
    template <>
    struct traits_t<false> {
        using mapped_type = T;
        using value_type = std::pair<Key, T>;
    };
    using key_type = Key;
    using key_compare = Compare;
    using value_type = traits_t<no_mapped_container>::value_type;
    using size_type = KeyContainer::size_type;
    using difference_type = KeyContainer::difference_type;
    using key_container_type = KeyContainer;
    using mapped_container_type = MappedContainer;
    using iterator = iterator_impl<typename key_container_type::iterator, typename mapped_container_type::iterator,
                                   std::conditional_t<no_mapped_container, const Key &, std::pair<const Key &, T &>>>;
    using const_iterator =
        iterator_impl<typename key_container_type::const_iterator, typename mapped_container_type::const_iterator,
                      std::conditional_t<no_mapped_container, const Key &, std::pair<const Key &, const T &>>>;

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

        Derived operator+(difference_type n) const { return Derived(keys_it_ + n, values_it_ + n); }

        Derived operator-(difference_type n) const { return Derived(keys_it_ - n, values_it_ - n); }

        bool operator==(const iterator_impl_base &other) const { return keys_it_ == other.keys_it_; }

        std::strong_ordering operator<=>(iterator_impl_base &other) const { return keys_it_ <=> other.keys_it_; }

    protected:
        KeysIterator keys_it_;
        ValuesIterator values_it_;
    };

    template <typename KeysIterator, typename ValuesIterator, typename ValueType>
    class iterator_impl : public iterator_impl_base<iterator_impl<KeysIterator, ValuesIterator, ValueType>,
                                                    KeysIterator, ValuesIterator> {
    public:
        using iterator_impl_base<iterator_impl, KeysIterator, ValuesIterator>::iterator_impl_base;
        ValueType operator*() const
        {
            if constexpr (no_mapped_container) {
                return *iterator_impl_base<iterator_impl, KeysIterator, ValuesIterator>::keys_it_;
            }
            else {
                return {*iterator_impl_base<iterator_impl, KeysIterator, ValuesIterator>::keys_it_,
                        *iterator_impl_base<iterator_impl, KeysIterator, ValuesIterator>::values_it_};
            }
        }
    };

public:
    struct containers {
        key_container_type keys;
        NO_UNIQUE_ADDRESS mapped_container_type values;
    };

    iterator begin() noexcept { return iterator(containers_.keys.begin(), containers_.values.begin()); }

    [[nodiscard]] const_iterator begin() const noexcept
    {
        return const_iterator(containers_.keys.begin(), containers_.values.begin());
    }

    iterator end() noexcept { return iterator(containers_.keys.end(), containers_.values.end()); }

    [[nodiscard]] const_iterator end() const noexcept
    {
        return const_iterator(containers_.keys.end(), containers_.values.end());
    }

    iterator find(const key_type &key)
    {
        auto first = containers_.keys.begin();
        auto last = containers_.keys.end();
        auto it = std::lower_bound(first, last, key, compare_);
        if (it != last && !compare_(*it, key) && !compare_(key, *it)) {
            auto offset = static_cast<difference_type>(it - first);
            return iterator(it, containers_.values.begin() + offset);
        }
        return end();
    }

    const_iterator find(const key_type &key) const
    {
        auto first = containers_.keys.begin();
        auto last = containers_.keys.end();
        auto it = std::lower_bound(first, last, key, compare_);
        if (it != last && !compare_(*it, key) && !compare_(key, *it)) {
            auto offset = static_cast<difference_type>(it - first);
            return const_iterator(it, containers_.values.begin() + offset);
        }
        return end();
    }

private:
    containers containers_;
    NO_UNIQUE_ADDRESS key_compare compare_;
};
};  // namespace brstd
