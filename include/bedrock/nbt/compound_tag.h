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

#include <algorithm>
#include <map>
#include <numeric>
#include <string>
#include <utility>
#include <variant>

#include "bedrock/bedrock.h"
#include "bedrock/nbt/byte_array_tag.h"
#include "bedrock/nbt/byte_tag.h"
#include "bedrock/nbt/double_tag.h"
#include "bedrock/nbt/end_tag.h"
#include "bedrock/nbt/float_tag.h"
#include "bedrock/nbt/int64_tag.h"
#include "bedrock/nbt/int_array_tag.h"
#include "bedrock/nbt/int_tag.h"
#include "bedrock/nbt/list_tag.h"
#include "bedrock/nbt/short_tag.h"
#include "bedrock/nbt/string_tag.h"
#include "bedrock/nbt/tag.h"

class CompoundTagVariant;

class CompoundTag : public Tag {
public:
    CompoundTag() = default;
    void write(IDataOutput &output) const override;
    Bedrock::Result<void> load(IDataInput &input) override;
    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] Type getId() const override;
    [[nodiscard]] bool equals(const Tag &other) const override;
    [[nodiscard]] std::unique_ptr<Tag> copy() const override;
    [[nodiscard]] std::uint64_t hash() const override;

    [[nodiscard]] bool contains(std::string_view key) const;
    [[nodiscard]] bool contains(std::string_view key, Tag::Type type) const;
    [[nodiscard]] const Tag *get(std::string_view key) const;
    std::uint8_t &putByte(std::string name, std::uint8_t value);
    void putBoolean(std::string name, bool value);
    float &putFloat(std::string name, float value);
    std::int32_t &putInt(std::string name, std::int32_t value);
    std::int64_t &putInt64(std::string name, std::int64_t value);
    std::string &putString(std::string name, std::string value);
    CompoundTag &putCompound(std::string name, CompoundTag value);

private:
    std::map<std::string, CompoundTagVariant, std::less<>> tags_;  // +8
};
BEDROCK_STATIC_ASSERT_SIZE(CompoundTag, 24, 32);

using CompoundTagVariantType = std::variant<EndTag, ByteTag, ShortTag, IntTag, Int64Tag, FloatTag, DoubleTag,
                                            ByteArrayTag, StringTag, ListTag, CompoundTag, IntArrayTag>;

class CompoundTagVariant : public CompoundTagVariantType {
public:
    using CompoundTagVariantType::CompoundTagVariantType;
    [[nodiscard]] const Tag *get() const
    {
        return std::visit([](auto &&arg) -> const Tag * { return &arg; }, *this);
    }
};
BEDROCK_STATIC_ASSERT_SIZE(CompoundTagVariant, 48, 48);
