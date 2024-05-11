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

#include <map>
#include <string>

#include "bedrock/bedrock.h"
#include "bedrock/nbt/tag.h"

class CompoundTagVariant;

class CompoundTag : public Tag {
public:
    BEDROCK_API void putBoolean(std::string, bool);
    BEDROCK_API std::uint8_t &putByte(std::string, std::uint8_t);
    BEDROCK_API std::int16_t &putShort(std::string, std::int16_t);
    BEDROCK_API std::int32_t &putInt(std::string, std::int32_t);
    BEDROCK_API std::int64_t &putInt64(std::string, std::int64_t);

private:
    std::map<std::string, CompoundTagVariant> tags_;  // +8
};
BEDROCK_STATIC_ASSERT_SIZE(CompoundTag, 24, 32);
