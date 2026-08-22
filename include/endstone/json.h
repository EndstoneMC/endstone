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

namespace endstone {

/** Any JSON value: an object, an array, a string, a number, a boolean, or null. */
using JsonValue = nlohmann::json;

/** A JSON object, mapping string keys to JSON values. */
using JsonObject = nlohmann::json::object_t;

/** A JSON array, a sequence of JSON values. */
using JsonArray = nlohmann::json::array_t;

}  // namespace endstone
