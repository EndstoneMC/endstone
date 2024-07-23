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

#include <nlohmann/json.hpp>

#include "value.h"

namespace Json {
inline nlohmann::json to_nlohmann(const Json::Value &value)  // NOLINT
{
    nlohmann::json result;
    switch (value.type()) {
    case Json::nullValue:
        break;
    case Json::intValue: {
        result = value.asInt64();
        break;
    }
    case Json::uintValue: {
        result = value.asUInt64();
        break;
    }
    case Json::realValue: {
        result = value.asDouble();
        break;
    }
    case Json::stringValue: {
        result = value.asString();
        break;
    }
    case Json::booleanValue: {
        result = value.asBool();
        break;
    }
    case Json::arrayValue: {
        for (int i = 0; i < value.size(); i++) {
            result.push_back(to_nlohmann(value[i]));
        }
        break;
    }
    case Json::objectValue: {
        auto members = value.getMemberNames();
        for (auto &member : members) {
            result[member] = to_nlohmann(value[member.c_str()]);
        }
        break;
    }
    default:
        throw std::runtime_error("Unexpected type of Json::Value");
    }
    return result;
}
}  // namespace Json
