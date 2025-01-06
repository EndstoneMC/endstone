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

#include "bedrock/deps/json/value.h"
#include "bedrock/util/new_type.h"

NLOHMANN_JSON_NAMESPACE_BEGIN
template <>
struct adl_serializer<Json::Value> {
    static void to_json(json &j, const Json::Value &value)
    {
        switch (value.type()) {
        case Json::nullValue:
            break;
        case Json::intValue: {
            j = value.asInt64();
            break;
        }
        case Json::uintValue: {
            j = value.asUInt64();
            break;
        }
        case Json::realValue: {
            j = value.asDouble();
            break;
        }
        case Json::stringValue: {
            j = value.asString();
            break;
        }
        case Json::booleanValue: {
            j = value.asBool();
            break;
        }
        case Json::arrayValue: {
            for (int i = 0; i < value.size(); i++) {
                j.push_back(value[i]);
            }
            break;
        }
        case Json::objectValue: {
            auto members = value.getMemberNames();
            for (auto &member : members) {
                j[member] = value[member.c_str()];
            }
            break;
        }
        }
    }
};
NLOHMANN_JSON_NAMESPACE_END

template <typename ValueType>
void to_json(nlohmann::json &j, const NewType<ValueType> &t)
{
    j = t.value;
}
