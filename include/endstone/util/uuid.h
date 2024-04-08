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

#include <array>
#include <cstddef>
#include <cstdint>

namespace endstone {

/**
 * Implementation of Universally Unique Identifier (UUID)
 *
 * Adapted from https://github.com/boostorg/uuid/blob/develop/include/boost/uuid/uuid.hpp
 */
class UUID {
public:
    typedef std::uint8_t value_type;
    typedef std::uint8_t &reference;
    typedef std::uint8_t const &const_reference;
    typedef std::uint8_t *iterator;
    typedef uint8_t const *const_iterator;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    static constexpr size_type static_size() noexcept
    {
        return 16;
    }

public:
    iterator begin() noexcept
    {
        return data;
    }

    const_iterator begin() const noexcept
    {
        return data;
    }

    iterator end() noexcept
    {
        return data + size();
    }

    const_iterator end() const noexcept
    {
        return data + size();
    }

    constexpr size_type size() const noexcept
    {
        return static_size();
    }

    bool is_nil() const noexcept
    {
        for (std::size_t i = 0; i < sizeof(data); ++i) {
            if (data[i] != 0U) {
                return false;
            }
        }
        return true;
    }

    int version() const noexcept
    {
        // version is stored in octet 9 which is index 6, since indexes count backwards
        uint8_t octet9 = data[6];
        if ((octet9 & 0xF0) == 0x10) {
            return 1;
        }
        else if ((octet9 & 0xF0) == 0x20) {
            return 2;
        }
        else if ((octet9 & 0xF0) == 0x30) {
            return 3;
        }
        else if ((octet9 & 0xF0) == 0x40) {
            return 4;
        }
        else if ((octet9 & 0xF0) == 0x50) {
            return 5;
        }
        else {
            return -1;
        }
    }

    void swap(UUID &rhs) noexcept
    {
        UUID tmp = *this;
        *this = rhs;
        rhs = tmp;
    }

public:
    std::uint8_t data[16];
};

inline bool operator<(UUID const &lhs, UUID const &rhs) noexcept
{
    return memcmp(lhs.data, rhs.data, sizeof(lhs.data)) < 0;
}

inline bool operator>(UUID const &lhs, UUID const &rhs) noexcept
{
    return rhs < lhs;
}
inline bool operator<=(UUID const &lhs, UUID const &rhs) noexcept
{
    return !(rhs < lhs);
}

inline bool operator>=(UUID const &lhs, UUID const &rhs) noexcept
{
    return !(lhs < rhs);
}

inline void swap(UUID &lhs, UUID &rhs) noexcept
{
    lhs.swap(rhs);
}

inline std::size_t hash_value(UUID const &u) noexcept
{
    std::size_t seed = 0;
    for (UUID::const_iterator i = u.begin(), e = u.end(); i != e; ++i) {
        seed ^= static_cast<std::size_t>(*i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    return seed;
}

}  // namespace endstone

namespace std {
template <>
struct hash<endstone::UUID> {
    std::size_t operator()(const endstone::UUID &value) const noexcept
    {
        return endstone::hash_value(value);
    }
};
}  // namespace std
