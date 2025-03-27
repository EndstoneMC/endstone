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
#include "bedrock/core/math/vec3.h"
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
    using StringView = std::string_view;

    CompoundTag() = default;
    void write(IDataOutput &output) const override;
    Bedrock::Result<void> load(IDataInput &input) override;
    [[nodiscard]] std::string toString() const override;
    [[nodiscard]] Type getId() const override;
    [[nodiscard]] bool equals(const Tag &other) const override;
    [[nodiscard]] std::unique_ptr<Tag> copy() const override;
    [[nodiscard]] std::size_t hash() const override;

    void getAllTags(std::vector<Tag *> &);

    Tag &put(std::string name, Tag &&tag);
    Tag *put(std::string name, std::unique_ptr<Tag> tag);
    std::uint8_t &putByte(std::string name, std::uint8_t value);
    std::int16_t &putShort(std::string name, std::int16_t value);
    std::int32_t &putInt(std::string name, std::int32_t value);
    std::int64_t &putInt64(std::string name, std::int64_t value);
    float &putFloat(std::string name, float value);
    double &putDouble(std::string name, double value);
    std::string &putString(std::string name, std::string value);
    ByteArrayTag::ArrayData &putByteArray(std::string, ByteArrayTag::ArrayData);
    CompoundTag &putCompound(std::string, CompoundTag);
    CompoundTag *putCompound(std::string, std::unique_ptr<CompoundTag>);
    void putBoolean(std::string name, bool value);

    [[nodiscard]] const Tag *get(StringView key) const;
    [[nodiscard]] Tag *get(StringView key);
    [[nodiscard]] std::uint8_t getByte(StringView) const;
    [[nodiscard]] const ByteTag *getByteTag(StringView) const;
    ByteTag *getByteTag(StringView);
    [[nodiscard]] std::int16_t getShort(StringView) const;
    [[nodiscard]] const ShortTag *getShortTag(StringView) const;
    ShortTag *getShortTag(StringView);
    [[nodiscard]] int getInt(StringView name) const;
    [[nodiscard]] const IntTag *getIntTag(StringView name) const;
    IntTag *getIntTag(StringView);
    [[nodiscard]] std::int64_t getInt64(StringView) const;
    [[nodiscard]] const Int64Tag *getInt64Tag(StringView) const;
    Int64Tag *getInt64Tag(StringView);
    [[nodiscard]] float getFloat(StringView) const;
    [[nodiscard]] const FloatTag *getFloatTag(StringView) const;
    FloatTag *getFloatTag(StringView);
    [[nodiscard]] double getDouble(StringView) const;
    [[nodiscard]] const DoubleTag *getDoubleTag(StringView) const;
    DoubleTag *getDoubleTag(StringView);
    [[nodiscard]] const std::string &getString(StringView) const;
    [[nodiscard]] const StringTag *getStringTag(StringView) const;
    StringTag *getStringTag(StringView);
    [[nodiscard]] const ByteArrayTag::ArrayData &getByteArray(StringView) const;
    [[nodiscard]] const ByteArrayTag *getByteArrayTag(StringView) const;
    ByteArrayTag *getByteArrayTag(StringView);
    [[nodiscard]] const IntArrayTag::ArrayData &getIntArray(StringView) const;
    [[nodiscard]] const IntArrayTag *getIntArrayTag(StringView) const;
    IntArrayTag *getIntArrayTag(StringView);
    [[nodiscard]] const CompoundTag *getCompound(StringView) const;
    CompoundTag *getCompound(StringView);
    [[nodiscard]] const ListTag *getList(StringView) const;
    ListTag *getList(StringView);
    [[nodiscard]] bool getBoolean(StringView) const;
    [[nodiscard]] Vec3 getVec3(StringView) const;

    void append(const CompoundTag &);
    [[nodiscard]] bool contains(StringView) const;
    [[nodiscard]] bool contains(StringView, Type) const;
    [[nodiscard]] bool isEmpty() const;
    void clear();
    void deepCopy(const CompoundTag &other);
    [[nodiscard]] std::unique_ptr<CompoundTag> clone() const;
    void rename(StringView, std::string);
    bool remove(StringView);

    [[nodiscard]] TagMap::const_iterator begin() const;
    [[nodiscard]] TagMap::const_iterator end() const;
    [[nodiscard]] std::size_t size() const;

private:
    template <typename TAG>
    TAG *get(StringView key);

    template <typename TAG>
    const TAG *get(StringView key) const;

    TagMap tags_;  // +8
};
BEDROCK_STATIC_ASSERT_SIZE(CompoundTag, 24, 32);

#include "bedrock/nbt/compound_tag_variant.h"
