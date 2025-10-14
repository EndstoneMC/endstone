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

#include "bedrock/core/string/string_hash.h"

HashedString::HashedString(std::nullptr_t) : last_match_(nullptr) {}

HashedString::HashedString(const std::string &str) : str_hash_(computeHash(str)), str_(str), last_match_(nullptr) {}

HashedString::HashedString(char const *str) : str_hash_(computeHash(str)), str_(str), last_match_(nullptr) {}

HashedString::HashedString(const HashedString &other)
    : str_hash_(other.str_hash_), str_(other.str_), last_match_(nullptr)
{
}

HashedString::HashedString(HashedString &&other) noexcept
    : str_hash_(other.str_hash_), str_(std::move(other.str_)), last_match_(nullptr)
{
}

HashedString &HashedString::operator=(const HashedString &other)
{
    if (this != &other) {
        str_ = other.str_;
        str_hash_ = other.str_hash_;
        last_match_ = nullptr;
    }
    return *this;
}

HashedString &HashedString::operator=(HashedString &&other) noexcept
{
    str_ = other.str_;
    str_hash_ = other.str_hash_;
    last_match_ = nullptr;
    return *this;
}

HashType64 HashedString::getHash() const
{
    return str_hash_;
}

const std::string &HashedString::getString() const
{
    return str_;
}

const char *HashedString::c_str() const
{
    return str_.c_str();
}

bool HashedString::empty() const
{
    return str_.empty();
}

bool HashedString::operator==(const HashedString &other) const
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

bool HashedString::operator!=(const HashedString &other) const
{
    return !(*this == other);
}
