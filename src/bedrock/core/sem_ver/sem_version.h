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

class SemVersion {
public:
    struct any_version_constructor {};
    static any_version_constructor const AnyVersionConstructor;

    SemVersion();
    explicit SemVersion(any_version_constructor);
    SemVersion(std::uint16_t major, std::uint16_t minor, std::uint16_t patch, std::string pre_release,
               std::string build_meta);
    [[nodiscard]] const std::string &asString() const;
    [[nodiscard]] std::uint16_t getMajor() const;
    [[nodiscard]] std::uint16_t getMinor() const;
    [[nodiscard]] std::uint16_t getPatch() const;
    bool operator==(const SemVersion &version) const;

private:
    std::uint16_t major_{0};           // +0
    std::uint16_t minor_{0};           // +2
    std::uint16_t patch_{0};           // +4
    std::string pre_release_;          // +8
    std::string build_meta_;           // +40
    std::string full_version_string_;  // +72
    bool valid_version_{false};        // +104
    bool any_version_{false};          // +105
};
