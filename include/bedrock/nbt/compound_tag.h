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
using TagMap = std::map<std::string, CompoundTagVariant, std::less<>>;

class CompoundTag : public Tag {
public:
    CompoundTag();
    void write(IDataOutput &output) const override;
    Bedrock::Result<void> load(IDataInput &input) override;
    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] Type getId() const override;
    [[nodiscard]] bool equals(const Tag &other) const override;
    [[nodiscard]] std::unique_ptr<Tag> copy() const override;
    [[nodiscard]] std::size_t hash() const override;

    [[nodiscard]] std::unique_ptr<CompoundTag> clone() const;
    void deepCopy(const CompoundTag &other);
    [[nodiscard]] bool contains(std::string_view key) const;
    [[nodiscard]] bool contains(std::string_view key, Type type) const;
    [[nodiscard]] const Tag *get(std::string_view key) const;
    [[nodiscard]] const std::string &getString(std::string_view key) const;
    [[nodiscard]] const CompoundTag *getCompound(std::string_view key) const;
    Tag &put(std::string name, Tag &&tag);
    Tag *put(std::string name, std::unique_ptr<Tag> tag);
    void putBoolean(std::string name, bool value);
    std::uint8_t &putByte(std::string name, std::uint8_t value);
    std::int16_t &putShort(std::string name, std::int16_t value);
    std::int32_t &putInt(std::string name, std::int32_t value);
    std::int64_t &putInt64(std::string name, std::int64_t value);
    float &putFloat(std::string name, float value);
    double &putDouble(std::string name, double value);
    std::string &putString(std::string name, std::string value);
    CompoundTag &putCompound(std::string name, CompoundTag value);
    CompoundTag *putCompound(std::string name, std::unique_ptr<CompoundTag> value);
    TagMap::iterator begin();
    TagMap::iterator end();
    [[nodiscard]] TagMap::const_iterator begin() const;
    [[nodiscard]] TagMap::const_iterator end() const;
    [[nodiscard]] std::size_t size() const;
    [[nodiscard]] bool empty() const;

private:
    TagMap tags_;  // +8
};
BEDROCK_STATIC_ASSERT_SIZE(CompoundTag, 24, 32);

#include "bedrock/nbt/compound_tag_variant.h"
