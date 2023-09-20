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
        True,
        False,
        Visitor,
        NotVisitor,
        Member,
        NotMember,
        Operator,
        NotOperator,
    };

public:
    [[nodiscard]] bool hasPermission(const Permissible &permissible) const noexcept
    {
        return hasPermission(permissible.getRole());
    }

    [[nodiscard]] bool hasPermission(PermissibleRole role) const noexcept
    {
        switch (value_) {
        case Value::True:
            return true;
        case Value::False:
            return false;
        case Value::Visitor:
            return role == PermissibleRole::Visitor;
        case Value::NotVisitor:
            return role != PermissibleRole::Visitor;
        case Value::Member:
            return role == PermissibleRole::Member;
        case Value::NotMember:
            return role != PermissibleRole::Member;
        case Value::Operator:
            return role == PermissibleRole::Operator;
        case Value::NotOperator:
            return role != PermissibleRole::Operator;
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
    const static PermissionDefault True;
    const static PermissionDefault False;
    const static PermissionDefault Visitor;
    const static PermissionDefault NotVisitor;
    const static PermissionDefault Member;
    const static PermissionDefault NotMember;
    const static PermissionDefault Operator;
    const static PermissionDefault NotOperator;
};

inline const PermissionDefault PermissionDefault::True = {PermissionDefault::Value::True, {"true"}};
inline const PermissionDefault PermissionDefault::False = {PermissionDefault::Value::False, {"false"}};
inline const PermissionDefault PermissionDefault::Visitor = {PermissionDefault::Value::Visitor, {"visitor"}};
inline const PermissionDefault PermissionDefault::NotVisitor = {PermissionDefault::Value::NotVisitor,
                                                                {"not_visitor", "!visitor"}};
inline const PermissionDefault PermissionDefault::Member = {PermissionDefault::Value::Member, {"member"}};
inline const PermissionDefault PermissionDefault::NotMember = {PermissionDefault::Value::NotMember,
                                                               {"not_member", "!member"}};
inline const PermissionDefault PermissionDefault::Operator = {PermissionDefault::Value::Operator, {"op", "operator"}};
inline const PermissionDefault PermissionDefault::NotOperator = {PermissionDefault::Value::NotOperator,
                                                                 {"not_op", "!op", "not_operator", "!operator"}};
