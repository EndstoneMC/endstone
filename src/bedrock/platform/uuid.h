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

#include <cstdint>
#include <string>

#include "bedrock/bedrock.h"

namespace mce {
class UUID {
public:
    static UUID EMPTY;
    UUID() = default;
    UUID(uint64_t high, uint64_t low);
    [[nodiscard]] std::string asString() const;
    bool operator==(const UUID &other) const = default;
    bool operator!=(const UUID &other) const = default;
    bool operator<(const UUID &other) const;

    static bool canParse(std::string_view in);
    static UUID fromString(std::string_view in);

    std::uint64_t data[2]{0, 0};
};
BEDROCK_STATIC_ASSERT_SIZE(UUID, 16, 16);
}  // namespace mce
