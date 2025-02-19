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

#include <string>

#include <fmt/format.h>

#include "bedrock/core/string/static_optimized_string.h"

struct SemVersionAnyVersionType {};
static_assert(sizeof(SemVersionAnyVersionType) == 1);

template <typename StringType>
class SemVersionBase;

template <typename StringTypeLhs, typename StringTypeRhs>
bool operator==(const SemVersionBase<StringTypeLhs> &lhs, const SemVersionBase<StringTypeRhs> &rhs)
{
    if (lhs.isAnyVersion() && rhs.isAnyVersion()) {
        return true;
    }

    if (lhs.isAnyVersion() || rhs.isAnyVersion()) {
        return false;
    }

    if (lhs.getMajor() != rhs.getMajor() || lhs.getMinor() != rhs.getMinor() || lhs.getPatch() != rhs.getPatch()) {
        return false;
    }

    if (lhs.getPreRelease().length() != rhs.getPreRelease().length()) {
        return false;
    }

    if (std::memcmp(lhs.getPreRelease(), rhs.getPreRelease(), rhs.getPreRelease().length()) != 0) {
        return false;
    }
    return true;
}

template <typename StringTypeLhs, typename StringTypeRhs>
bool operator!=(const SemVersionBase<StringTypeLhs> &lhs, const SemVersionBase<StringTypeRhs> &rhs)
{
    return !(lhs == rhs);
}

template <typename StringTypeLhs, typename StringTypeRhs>
bool operator<(const SemVersionBase<StringTypeLhs> &lhs, const SemVersionBase<StringTypeRhs> &rhs)
{
    if (lhs.isAnyVersion()) {
        return false;
    }

    if (rhs.isAnyVersion()) {
        return true;
    }

    if (lhs.getMajor() != rhs.getMajor()) {
        return lhs.getMajor() < rhs.getMajor();
    }

    if (lhs.getMinor() != rhs.getMinor()) {
        return lhs.getMinor() < rhs.getMinor();
    }

    if (lhs.getPatch() != rhs.getPatch()) {
        return lhs.getPatch() < rhs.getPatch();
    }

    if (lhs.getPreRelease().length() == 0 && rhs.getPreRelease().length() != 0) {
        return false;
    }

    if (lhs.getPreRelease().length() != 0 && rhs.getPreRelease().length() == 0) {
        return true;
    }

    auto version_split = [](std::string_view str, const char delim = '.') -> std::vector<std::string> {
        std::vector<std::string> result;
        size_t start = 0;
        size_t end;
        while ((end = str.find(delim, start)) != std::string_view::npos) {
            if (end > start) {
                result.emplace_back(str.substr(start, end - start));
            }
            start = end + 1;
        }
        if (start < str.size()) {
            result.emplace_back(str.substr(start));
        }
        return result;
    };

    std::vector<std::string> lhs_parts = version_split(lhs.getPreRelease());
    std::vector<std::string> rhs_parts = version_split(rhs.getPreRelease());

    for (size_t i = 0; i < std::min(lhs_parts.size(), rhs_parts.size()); ++i) {
        bool lhs_numeric = std::ranges::all_of(lhs_parts[i], ::isdigit);
        bool rhs_numeric = std::ranges::all_of(rhs_parts[i], ::isdigit);

        if (lhs_numeric && rhs_numeric) {
            if (std::stoi(lhs_parts[i]) != std::stoi(rhs_parts[i])) {
                return std::stoi(lhs_parts[i]) < std::stoi(rhs_parts[i]);
            }
        }
        else if (lhs_numeric) {
            return true;
        }
        else if (rhs_numeric) {
            return false;
        }
        else {
            if (lhs_parts[i] != rhs_parts[i]) {
                return lhs_parts[i] < rhs_parts[i];
            }
        }
    }
    return lhs_parts.size() < rhs_parts.size();
}

template <typename StringTypeLhs, typename StringTypeRhs>
bool operator<=(const SemVersionBase<StringTypeLhs> &lhs, const SemVersionBase<StringTypeRhs> &rhs)
{
    return lhs == rhs || lhs < rhs;
}

template <typename StringType>
class SemVersionBase {
public:
    inline static const SemVersionAnyVersionType AnyVersionConstructor;
    SemVersionBase() = default;
    explicit SemVersionBase(SemVersionAnyVersionType) : valid_version_(true), any_version_(true) {}
    explicit SemVersionBase(const std::uint16_t major, const std::uint16_t minor, const std::uint16_t patch,
                            StringType pre_release = {}, StringType build_meta = {})
        : major_(major), minor_(minor), patch_(patch), valid_version_(true), pre_release_(std::move(pre_release)),
          build_meta_(std::move(build_meta))
    {
    }

    [[nodiscard]] std::string asString() const
    {
        if (any_version_) {
            return "*";
        }

        auto full_version_string = fmt::format("{}.{}.{}", major_, minor_, patch_);
        if (pre_release_.length() > 0) {
            full_version_string += "-";
            full_version_string += pre_release_;
        }
        if (build_meta_.length() > 0) {
            full_version_string += "+";
            full_version_string += build_meta_;
        }
        return full_version_string;
    }

    [[nodiscard]] bool isValid() const
    {
        return valid_version_;
    }

    [[nodiscard]] bool isAnyVersion() const
    {
        return any_version_;
    }

    [[nodiscard]] std::uint16_t getMajor() const
    {
        return major_;
    }

    [[nodiscard]] std::uint16_t getMinor() const
    {
        return minor_;
    }

    [[nodiscard]] std::uint16_t getPatch() const
    {
        return patch_;
    }

    [[nodiscard]] StringType getPreRelease() const
    {
        return pre_release_;
    }

    [[nodiscard]] StringType getBuildMeta() const
    {
        return build_meta_;
    }

private:
    std::uint16_t major_{0};  // +0
    std::uint16_t minor_{0};  // +2
    std::uint16_t patch_{0};  // +4
    bool valid_version_{false};
    bool any_version_{false};
    StringType pre_release_;
    StringType build_meta_;
};

class SemVersion : public SemVersionBase<Bedrock::StaticOptimizedString> {
public:
    using SemVersionBase::SemVersionBase;
};
static_assert(sizeof(SemVersion) == 24);
