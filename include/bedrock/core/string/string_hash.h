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

#include <cstdint>
#include <string>

using HashType64 = std::uint64_t;

class HashedString {
public:
    // NOLINTBEGIN(*-explicit-constructor)
    HashedString(std::nullptr_t);
    HashedString(const std::string &str);
    HashedString(char const *str);
    // NOLINTEND(*-explicit-constructor)

    HashedString(const HashedString &other);
    HashedString(HashedString &&other) noexcept;

    HashedString &operator=(const HashedString &other);
    HashedString &operator=(HashedString &&other) noexcept;

    [[nodiscard]] HashType64 getHash() const;
    [[nodiscard]] const std::string &getString() const;
    [[nodiscard]] const char *c_str() const;  // NOLINT(*-identifier-naming)

    bool operator==(const HashedString &other) const;
    bool operator!=(const HashedString &other) const;

    static HashType64 computeHash(const std::string &str)
    {
        constexpr HashType64 offset_basis = 0xCBF29CE484222325;
        constexpr HashType64 prime = 0x100000001B3;

        if (str.empty()) {
            return 0;
        }

        HashType64 hash = offset_basis;
        for (char c : str) {
            hash *= prime;
            hash ^= static_cast<unsigned char>(c);
        }
        return hash;
    }

    static HashType64 computeHash(const char *str)
    {
        constexpr HashType64 offset_basis = 0xCBF29CE484222325;
        constexpr HashType64 prime = 0x100000001B3;

        if (!str || str[0] == '\0') {
            return 0;
        }

        HashType64 hash = offset_basis;
        for (int i = 0; str[i] != '\0'; i++) {
            hash *= prime;
            hash ^= static_cast<unsigned char>(str[i]);
        }
        return hash;
    }

private:
    HashType64 str_hash_{0};                  // +0
    std::string str_;                         // +8
    mutable const HashedString *last_match_;  // +40
};

namespace std {
template <>
struct hash<HashedString> {
    HashType64 operator()(const HashedString &value) const noexcept
    {
        return value.getHash();
    }
};
}  // namespace std
