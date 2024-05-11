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

#include "bedrock/nbt/compound_tag.h"

#include "endstone/detail/hook.h"

void CompoundTag::putBoolean(std::string key, bool value)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&CompoundTag::putBoolean, this, std::move(key), value);
}

std::uint8_t &CompoundTag::putByte(std::string key, std::uint8_t value)
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&CompoundTag::putByte, this, std::move(key), value);
}

std::int16_t &CompoundTag::putShort(std::string key, std::int16_t value)
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&CompoundTag::putShort, this, std::move(key), value);
}

std::int32_t &CompoundTag::putInt(std::string key, std::int32_t value)
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&CompoundTag::putInt, this, std::move(key), value);
}

std::int64_t &CompoundTag::putInt64(std::string key, std::int64_t value)
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&CompoundTag::putInt64, this, std::move(key), value);
}
