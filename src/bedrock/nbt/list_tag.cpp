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

#include "bedrock/nbt/list_tag.h"

#include "bedrock/nbt/byte_tag.h"
#include "bedrock/nbt/compound_tag.h"
#include "bedrock/nbt/double_tag.h"
#include "bedrock/nbt/float_tag.h"
#include "bedrock/nbt/int64_tag.h"
#include "bedrock/nbt/int_tag.h"
#include "bedrock/nbt/short_tag.h"
#include "bedrock/nbt/string_tag.h"
#include "bedrock/nbt/tag.h"

void ListTag::deleteChildren()
{
    while (!list_.empty()) {
        list_.back()->deleteChildren();
        list_.pop_back();
    }
}

void ListTag::write(IDataOutput &output) const
{
    output.writeByte(static_cast<std::uint8_t>(type_));
    output.writeInt(static_cast<std::int32_t>(list_.size()));
    for (const auto &data : list_) {
        data->write(output);
    }
}

Bedrock::Result<void> ListTag::load(IDataInput &input)
{
    auto byte_result = input.readByteResult();
    if (!byte_result.ignoreError()) {
        return BEDROCK_RETHROW(byte_result);
    }
    type_ = static_cast<Type>(byte_result.asExpected().value());

    auto int_result = input.readIntResult();
    if (!int_result.ignoreError()) {
        return BEDROCK_RETHROW(int_result);
    }

    const auto size = int_result.asExpected().value();
    list_.clear();
    list_.reserve(size);
    for (int i = 0; i < size; ++i) {
        auto tag_result = newTag(type_);
        if (!tag_result.ignoreError()) {
            return BEDROCK_RETHROW(tag_result);
        }
        auto tag = std::move(tag_result.discardError().value());
        tag->load(input);
        list_.push_back(std::move(tag));
    }
    return {};
}

std::string ListTag::toString() const
{
    return fmt::format("{} entries of type {}", list_.size(), Tag::getTagName(type_));
}

Tag::Type ListTag::getId() const
{
    return Type::List;
}

bool ListTag::equals(const Tag &other) const
{
    if (Tag::equals(other) && type_ == static_cast<const ListTag &>(other).type_) {
        return std::ranges::equal(list_, static_cast<const ListTag &>(other).list_,
                                  [](const auto &lhs, const auto &rhs) { return lhs->equals(*rhs); });
    }
    return false;
}

std::unique_ptr<Tag> ListTag::copy() const
{
    return copyList();
}

std::size_t ListTag::hash() const
{
    return boost::hash_range(list_.begin(), list_.end());
}

void ListTag::print(const std::string &string, PrintStream &stream) const
{
    Tag::print(string, stream);
    stream.print(string);
    stream.print("{\n");
    stream.print("   ");
    for (const auto &data : list_) {
        data->print(string, stream);
    }
    stream.print(string);
    stream.print("}\n");
}

Tag::Type ListTag::getType() const
{
    return type_;
}

void ListTag::add(std::unique_ptr<Tag> tag)
{
    type_ = tag->getId();
    list_.push_back(std::move(tag));
}

Tag *ListTag::get(int index) const
{
    if (index < 0 || index >= list_.size()) {
        return nullptr;
    }
    return list_[index].get();
}

float ListTag::getFloat(int index) const
{
    if (auto *tag = get(index); tag && tag->getId() == Type::Float) {
        return static_cast<FloatTag *>(tag)->data;
    }
    return 0;
}

int ListTag::getInt(int index) const
{
    if (auto *tag = get(index); tag && tag->getId() == Type::Int) {
        return static_cast<IntTag *>(tag)->data;
    }
    return 0;
}

double ListTag::getDouble(int index) const
{
    if (auto *tag = get(index); tag && tag->getId() == Type::Double) {
        return static_cast<DoubleTag *>(tag)->data;
    }
    return 0;
}

const std::string &ListTag::getString(int index) const
{
    static std::string empty_string;
    if (auto *tag = get(index); tag && tag->getId() == Type::String) {
        return static_cast<StringTag *>(tag)->data;
    }
    return empty_string;
}

std::int64_t ListTag::getInt64(int index) const
{
    if (auto *tag = get(index); tag && tag->getId() == Type::Int64) {
        return static_cast<Int64Tag *>(tag)->data;
    }
    return 0;
}

std::int16_t ListTag::getShort(int index) const
{
    if (auto *tag = get(index); tag && tag->getId() == Type::Short) {
        return static_cast<ShortTag *>(tag)->data;
    }
    return 0;
}

std::uint8_t ListTag::getByte(int index) const
{
    if (auto *tag = get(index); tag && tag->getId() == Type::Byte) {
        return static_cast<ByteTag *>(tag)->data;
    }
    return 0;
}

std::size_t ListTag::size() const
{
    return list_.size();
}

std::unique_ptr<ListTag> ListTag::copyList() const
{
    auto copy = std::make_unique<ListTag>();
    copy->type_ = type_;
    for (const auto &data : list_) {
        copy->list_.push_back(data->copy());
    }
    return copy;
}

const CompoundTag *ListTag::getCompound(int index) const
{
    if (const auto *tag = get(index); tag && tag->getId() == Type::Compound) {
        return static_cast<const CompoundTag *>(tag);
    }
    return nullptr;
}

CompoundTag *ListTag::getCompound(int index)
{
    if (auto *tag = get(index); tag && tag->getId() == Type::Compound) {
        return static_cast<CompoundTag *>(tag);
    }
    return nullptr;
}

void ListTag::erase(int index)
{
    if (index < 0 || index >= list_.size()) {
        return;
    }
    list_.erase(list_.begin() + index);
}

void ListTag::popBack()
{
    list_.pop_back();
}

void ListTag::forEachCompoundTag(std::function<void(const CompoundTag &)> func) const
{
    for (const auto &tag : list_) {
        if (tag->getId() == Type::Compound) {
            func(*static_cast<const CompoundTag *>(tag.get()));
        }
    }
}
