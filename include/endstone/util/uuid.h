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
#include <cstdint>
#include <cstring>
#include <functional>
#include <string>

namespace endstone {

/**
 * Implementation of Universally Unique Identifier (UUID).
 *
 * Adapted from https://github.com/boostorg/uuid/blob/develop/include/boost/uuid/uuid.hpp
 */
class UUID {
public:
    /**
     * Returns an iterator to the first byte.
     *
     * @return Pointer to the first byte
     */
    std::uint8_t *begin() noexcept { return data; }

    /**
     * @copydoc begin()
     */
    [[nodiscard]] const uint8_t *begin() const noexcept { return data; }

    /**
     * Returns an iterator past the last byte.
     *
     * @return Pointer past the last byte
     */
    std::uint8_t *end() noexcept { return data + size(); }

    /**
     * @copydoc end()
     */
    [[nodiscard]] const uint8_t *end() const noexcept { return data + size(); }

    /**
     * Returns the number of bytes in a UUID.
     *
     * @return The size in bytes (always 16)
     */
    [[nodiscard]] static constexpr std::size_t size() noexcept { return 16; }

    /**
     * Checks whether this UUID is nil (all bytes are zero).
     *
     * @return true if every byte is zero, false otherwise
     */
    [[nodiscard]] bool isNil() const noexcept
    {
        return std::all_of(std::begin(data), std::end(data), [](const auto &val) { return val == 0U; });
    }

    /**
     * Returns the version number encoded in this UUID.
     *
     * @return The UUID version (1-5), or -1 if it is not a recognized version
     */
    [[nodiscard]] int version() const noexcept
    {
        // version is stored in octet 9 which is index 6, since indexes count backwards
        uint8_t octet9 = data[6];
        if ((octet9 & 0xF0) == 0x10) {
            return 1;
        }
        if ((octet9 & 0xF0) == 0x20) {
            return 2;
        }
        if ((octet9 & 0xF0) == 0x30) {
            return 3;
        }
        if ((octet9 & 0xF0) == 0x40) {
            return 4;
        }
        if ((octet9 & 0xF0) == 0x50) {
            return 5;
        }
        return -1;
    }

    /**
     * Swaps the contents of this UUID with another.
     *
     * @param rhs The UUID to swap with
     */
    void swap(UUID &rhs) noexcept
    {
        UUID tmp = *this;
        *this = rhs;
        rhs = tmp;
    }

    /**
     * Returns the canonical hyphenated string representation of this UUID.
     *
     * @return The UUID formatted as a 36-character string
     */
    [[nodiscard]] std::string str() const
    {
        std::string result;
        result.reserve(36);

        static const char *chars = "0123456789abcdef";

        for (size_t i = 0; i < 16; ++i) {
            result.push_back(chars[(data[i] >> 4) & 0x0F]);
            result.push_back(chars[data[i] & 0x0F]);
            if (i == 3 || i == 5 || i == 7 || i == 9) {
                result.push_back('-');
            }
        }

        return result;
    }
    std::uint8_t data[16] = {0};
};
static_assert(sizeof(endstone::UUID) == endstone::UUID::size());

inline bool operator==(UUID const &lhs, UUID const &rhs) noexcept
{
    return memcmp(lhs.data, rhs.data, sizeof(lhs.data)) == 0;
}

inline bool operator<(UUID const &lhs, UUID const &rhs) noexcept
{
    return memcmp(lhs.data, rhs.data, sizeof(lhs.data)) < 0;
}

inline bool operator!=(UUID const &lhs, UUID const &rhs) noexcept
{
    return !(lhs == rhs);
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
    for (unsigned char i : u) {
        seed ^= static_cast<std::size_t>(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    return seed;
}

}  // namespace endstone

namespace std {
template <>
struct hash<endstone::UUID> {
    std::size_t operator()(const endstone::UUID &value) const noexcept { return endstone::hash_value(value); }
};
}  // namespace std
