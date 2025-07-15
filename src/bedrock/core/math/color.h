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

#include <iomanip>
#include <sstream>
#include <string>
#include <string_view>

namespace mce {
class Color {
public:
    static const Color WHITE;

    [[nodiscard]] std::string toHexString() const;
    static Color fromHexString(const std::string &hex_string);

    Color() = default;
    Color(float, float, float, float);

    float r = 1.0;
    float g = 1.0;
    float b = 1.0;
    float a = 1.0;
};
}  // namespace mce

using Color = mce::Color;
