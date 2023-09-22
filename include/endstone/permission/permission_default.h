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

#include "endstone/permission/permissible.h"

class PermissionDefault {

    enum class Value {
        Any,
        None,
        Operator,
        Owner
    };

public:
    [[nodiscard]] std::string_view getName() const noexcept
    {
        return names_[0];
    }

    [[nodiscard]] bool isGrantedFor(const Permissible &permissible) const noexcept
    {
        return isGrantedFor(permissible.getRole());
    }

    [[nodiscard]] bool isGrantedFor(PermissibleRole role) const noexcept
    {
        switch (value_) {
        case Value::Any:
            return true;
        case Value::None:
            return false;
        case Value::Operator:
            return role >= PermissibleRole::Operator;
        case Value::Owner:
            return role >= PermissibleRole::Owner;
        default:
            return false;
        }
    }

    bool operator==(const PermissionDefault &rhs) const noexcept
    {
        return value_ == rhs.value_;
    }

    bool operator!=(const PermissionDefault &rhs) const noexcept
    {
        return !(rhs == *this);
    }

    static const PermissionDefault *getByName(const std::string &name) noexcept
    {
        std::string lower_name = name;
        std::transform(lower_name.begin(), lower_name.end(), lower_name.begin(), [](unsigned char c) {
            return std::tolower(c);
        });
        lower_name = std::regex_replace(lower_name, std::regex("[^a-z!]"), "");
        auto it = mLookupByName.find(lower_name);
        return it != mLookupByName.end() ? it->second : nullptr;
    }

private:
    PermissionDefault(const Value value, const std::vector<std::string> &names) noexcept : value_(value), names_(names)
    {
        for (const auto &name : names) {
            mLookupByName.insert({name, this});
        }
    }

    Value value_;
    std::vector<std::string> names_;

    inline static std::unordered_map<std::string, PermissionDefault *> mLookupByName;

public:
    const static PermissionDefault Any;
    const static PermissionDefault None;
    const static PermissionDefault Operator;
    const static PermissionDefault Owner;
};

inline const PermissionDefault PermissionDefault::Any = {PermissionDefault::Value::Any,  //
                                                         {"any", "true", "player", "member", "visitor"}};
inline const PermissionDefault PermissionDefault::None = {PermissionDefault::Value::None,  //
                                                          {"none", "false"}};
inline const PermissionDefault PermissionDefault::Operator = {PermissionDefault::Value::Operator,  //
                                                              {"op", "operator", "admin", "moderator"}};
inline const PermissionDefault PermissionDefault::Owner = {PermissionDefault::Value::Owner,  //
                                                           {"owner", "server", "console"}};
