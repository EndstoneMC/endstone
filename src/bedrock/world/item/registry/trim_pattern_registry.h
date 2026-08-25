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

#include <optional>
#include <unordered_map>

#include "bedrock/bedrock.h"
#include "bedrock/core/string/string_hash.h"

struct TrimPattern {
    HashedString pattern_id;
    HashedString item_name;
};
BEDROCK_STATIC_ASSERT_SIZE(TrimPattern, 96, 96);

class TrimPatternRegistry {
public:
    [[nodiscard]] std::optional<HashedString> getPatternIdByItem(const HashedString &item_id) const;

private:
    std::unordered_map<HashedString, HashedString> template_item_to_pattern_id_;
    std::unordered_map<HashedString, HashedString> pattern_id_to_template_item_;
};
BEDROCK_STATIC_ASSERT_SIZE(TrimPatternRegistry, 128, 128);
