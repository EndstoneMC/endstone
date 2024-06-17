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
    HashedString(std::nullptr_t) {}  // NOLINT(*-explicit-constructor)
    explicit HashedString(const std::string &str) : str_(str)
    {
        str_hash_ = std::hash<std::string>()(str);
    }
    explicit HashedString(char const *str) : HashedString(std::string(str)) {}

    [[nodiscard]] std::size_t getHash() const
    {
        return str_hash_;
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
