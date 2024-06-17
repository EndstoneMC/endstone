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

class HashedString {
public:
    HashedString(std::nullptr_t) {}                                                   // NOLINT(*-explicit-constructor)
    HashedString(const std::string &str) : str_(str), str_hash_(computeHash(str)) {}  // NOLINT(*-explicit-constructor)
    HashedString(char const *str) : str_(str), str_hash_(computeHash(str)) {}         // NOLINT(*-explicit-constructor)

    HashedString(const HashedString &other) : str_(other.str_), str_hash_(other.str_hash_), last_match_(nullptr) {}
    HashedString(HashedString &&other) noexcept
        : str_(std::move(other.str_)), str_hash_(other.str_hash_), last_match_(nullptr)
    {
    }

    HashedString &operator=(const HashedString &other)
    {
        if (this != &other) {
            str_ = other.str_;
            str_hash_ = other.str_hash_;
            last_match_ = nullptr;
        }
        return *this;
    }

    HashedString &operator=(HashedString &&other) noexcept
    {
        str_ = other.str_;
        str_hash_ = other.str_hash_;
        last_match_ = nullptr;
        return *this;
    }

    [[nodiscard]] std::size_t getHash() const
    {
        return str_hash_;
    }

    [[nodiscard]] const std::string &getString() const
    {
        return str_;
    }

    [[nodiscard]] const char *c_str() const  // NOLINT(*-identifier-naming)
    {
        return str_.c_str();
    }

    bool constexpr operator==(const HashedString &other) const
    {
        if (str_hash_ == other.str_hash_) {
            if (last_match_ == &other && other.last_match_ == this) {
                return true;
            }
            if (str_ == other.str_) {
                last_match_ = &other;
                other.last_match_ = this;
                return true;
            }
        }
        last_match_ = nullptr;
        return false;
    }

    bool constexpr operator!=(const HashedString &other) const
    {
        return !(*this == other);
    }

    static std::size_t computeHash(const std::string &str)
    {
        constexpr uint64_t offset_basis = 0xCBF29CE484222325;
        constexpr uint64_t prime = 0x100000001B3;

        if (str.empty()) {
            return 0;
        }

        std::size_t hash = offset_basis;
        for (char c : str) {
            hash *= prime;
            hash ^= static_cast<unsigned char>(c);
        }
        return hash;
    }

    static std::size_t computeHash(const char *str)
    {
        constexpr uint64_t offset_basis = 0xCBF29CE484222325;
        constexpr uint64_t prime = 0x100000001B3;

        if (!str || str[0] == '\0') {
            return 0;
        }

        std::size_t hash = offset_basis;
        for (int i = 0; str[i] != '\0'; i++) {
            hash *= prime;
            hash ^= static_cast<unsigned char>(str[i]);
        }
        return hash;
    }

private:
    std::size_t str_hash_{0};                 // +0
    std::string str_;                         // +8
    mutable const HashedString *last_match_;  // +40
};

namespace std {
template <>
struct hash<HashedString> {
    std::size_t operator()(const HashedString &value) const noexcept
    {
        return value.getHash();
    }
};
}  // namespace std
