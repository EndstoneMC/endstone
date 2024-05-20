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

class CompoundTag : public Tag {
public:
    CompoundTag() = default;
    void write(IDataOutput &output) const override
    {
        // TODO(nbt): fixme
        throw std::runtime_error("Not implemented");
    }
    Bedrock::Result<void> load(IDataInput &input) override
    {
        // TODO(nbt): fixme
        throw std::runtime_error("Not implemented");
    }
    [[nodiscard]] std::string toString() const override
    {
        return fmt::format("{} entries", tags_.size());
    }
    [[nodiscard]] Type getId() const override
    {
        return Type::Compound;
    }
    [[nodiscard]] bool equals(const Tag &other) const override
    {
        // TODO(nbt): fixme
        throw std::runtime_error("Not implemented");
    }
    [[nodiscard]] std::unique_ptr<Tag> copy() const override
    {
        // TODO(nbt): fixme
        throw std::runtime_error("Not implemented");
    }
    [[nodiscard]] std::uint64_t hash() const override
    {
        // TODO(nbt): fixme
        throw std::runtime_error("Not implemented");
    }

private:
    std::map<std::string, CompoundTagVariant> tags_;  // +8
};
BEDROCK_STATIC_ASSERT_SIZE(CompoundTag, 24, 32);

class CompoundTagVariant : public std::variant<EndTag, CompoundTag, ShortTag, IntTag, Int64Tag, FloatTag, DoubleTag,
                                               ByteArrayTag, StringTag, ListTag, CompoundTag, IntArrayTag> {
public:
    CompoundTagVariant() = delete;
    CompoundTagVariant(const CompoundTagVariant &other) = default;
    CompoundTagVariant &operator=(const CompoundTagVariant &other) = default;
    CompoundTagVariant(CompoundTagVariant &&other) = default;
    CompoundTagVariant &operator=(CompoundTagVariant &&other) = default;

    [[nodiscard]] const Tag *get() const
    {
        return std::visit([](auto &&arg) -> const Tag * { return &arg; }, *this);
    }
};
BEDROCK_STATIC_ASSERT_SIZE(CompoundTagVariant, 48, 48);
