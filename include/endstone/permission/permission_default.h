// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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
#include <regex>
#include <string>
#include <unordered_map>
#include <vector>

enum class PermissionLevel {
    Any,
    Member,
    Operator,
    Owner
};

class PermissionDefault {
public:
    // Delete copy constructor and copy assignment operator
    PermissionDefault(const PermissionDefault &) = delete;
    PermissionDefault &operator=(const PermissionDefault &) = delete;

    // Delete move constructor and move assignment operator
    PermissionDefault(PermissionDefault &&) = delete;
    PermissionDefault &operator=(PermissionDefault &&) = delete;

    [[nodiscard]] bool hasPermission(const PermissionLevel &level) const
    {
        return level >= level_;
    }

    static const PermissionDefault *getByName(const std::string &name)
    {
        std::string lower_name = name;
        std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        lower_name = std::regex_replace(lower_name, std::regex("[^a-z!]"), "");
        auto it = lookup_.find(lower_name);
        return it != lookup_.end() ? it->second : nullptr;
    }

private:
    PermissionDefault(const std::vector<std::string> &names, const PermissionLevel level) : names_(names), level_(level)
    {
        for (const auto &name : names) {
            lookup_.insert({name, this});
        }
    }

    const std::vector<std::string> names_;
    const PermissionLevel level_;

    inline static std::unordered_map<std::string, PermissionDefault *> lookup_;
};
