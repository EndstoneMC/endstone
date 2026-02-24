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

#include <algorithm>
#include <utility>

#include "bedrock/nbt/compound_tag_variant.h"
#include "bedrock/nbt/nbt_io.h"

void CompoundTag::write(IDataOutput &output) const
{
    for (const auto &[key, value] : tags_) {
        NbtIo::writeNamedTag(key, *value.get(), output);
    }
    output.writeByte(static_cast<std::uint8_t>(Type::End));
}

Bedrock::Result<void> CompoundTag::load(IDataInput &input)
{
    tags_.clear();
    while (input.numBytesLeft()) {
        std::string name;
        auto tag_result = NbtIo::readNamedTag(input, name);
        if (!tag_result.ignoreError()) {
            return BEDROCK_RETHROW(tag_result);
        }
        if (tag_result.discardError().value()->getId() == Type::End) {
            break;
        }
        put(name, std::move(tag_result.discardError().value()));
    }

    return {};
}

std::string CompoundTag::toString() const
{
    return fmt::format("{} entries", tags_.size());
}

Tag::Type CompoundTag::getId() const
{
    return Type::Compound;
}

bool CompoundTag::equals(const Tag &other) const
{
    if (!Tag::equals(other)) {
        return false;
    }

    const auto &other_tag = static_cast<const CompoundTag &>(other);
    return std::ranges::all_of(tags_, [&](const auto &kv) {
        const auto *tag = other_tag.get(kv.first);
        return tag && kv.second.get()->equals(*tag);
    });
}

std::unique_ptr<Tag> CompoundTag::copy() const
{
    return clone();
}

std::uint64_t CompoundTag::hash() const
{
    static std::hash<std::string> hasher;
    std::size_t seed = 0;
    for (const auto &[key, value] : tags_) {
        seed ^= hasher(key) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= value.get()->hash() + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

Tag &CompoundTag::put(std::string name, Tag &&tag)
{
    return tags_[name].emplace(std::forward<Tag>(tag));
}

Tag *CompoundTag::put(std::string name, std::unique_ptr<Tag> tag)
{
    if (!tag) {
        return nullptr;
    }
    return &tags_[name].emplace(std::move(*tag));
}

std::uint8_t &CompoundTag::putByte(std::string name, std::uint8_t value)
{
    auto &tag = tags_[name].emplace(ByteTag(value));
    return static_cast<ByteTag &>(tag).data;
}

std::int16_t &CompoundTag::putShort(std::string name, std::int16_t value)
{
    auto &tag = tags_[name].emplace(ShortTag(value));
    return static_cast<ShortTag &>(tag).data;
}

std::int32_t &CompoundTag::putInt(std::string name, std::int32_t value)
{
    auto &tag = tags_[name].emplace(IntTag(value));
    return static_cast<IntTag &>(tag).data;
}

std::int64_t &CompoundTag::putInt64(std::string name, std::int64_t value)
{
    auto &tag = tags_[name].emplace(Int64Tag(value));
    return static_cast<Int64Tag &>(tag).data;
}

float &CompoundTag::putFloat(std::string name, float value)
{
    auto &tag = tags_[name].emplace(FloatTag(value));
    return static_cast<FloatTag &>(tag).data;
}

double &CompoundTag::putDouble(std::string name, double value)
{
    auto &tag = tags_[name].emplace(DoubleTag(value));
    return static_cast<DoubleTag &>(tag).data;
}

std::string &CompoundTag::putString(std::string name, std::string value)
{
    auto &tag = tags_[name].emplace(StringTag(std::move(value)));
    return static_cast<StringTag &>(tag).data;
}

CompoundTag &CompoundTag::putCompound(std::string name, CompoundTag value)
{
    auto &tag = tags_[name].emplace(std::move(value));
    return static_cast<CompoundTag &>(tag);
}

CompoundTag *CompoundTag::putCompound(std::string name, std::unique_ptr<CompoundTag> value)
{
    if (!value) {
        return nullptr;
    }
    return static_cast<CompoundTag *>(&tags_[name].emplace(std::move(*value)));
}

void CompoundTag::putBoolean(std::string name, bool value)
{
    tags_[name].emplace(ByteTag(value ? 1 : 0));
}

const Tag *CompoundTag::get(StringView key) const
{
    auto it = tags_.find(key);
    if (it != tags_.end()) {
        return it->second.get();
    }
    return nullptr;
}

Tag *CompoundTag::get(StringView key)
{
    auto it = tags_.find(key);
    if (it != tags_.end()) {
        return it->second.get();
    }
    return nullptr;
}

std::uint8_t CompoundTag::getByte(StringView key) const
{
    if (const auto *tag = getByteTag(key)) {
        return tag->data;
    }
    return 0;
}

const ByteTag *CompoundTag::getByteTag(StringView key) const
{
    if (const auto *tag = get(key); tag) {
        if (tag->getId() == Type::Byte) {
            return static_cast<const ByteTag *>(tag);
        }
    }
    return nullptr;
}

int CompoundTag::getInt(StringView name) const
{
    if (const auto *tag = getIntTag(name)) {
        return tag->data;
    }
    return 0;
}

const IntTag *CompoundTag::getIntTag(StringView name) const
{
    if (const auto *tag = get(name); tag) {
        if (tag->getId() == Type::Int) {
            return static_cast<const IntTag *>(tag);
        }
    }
    return nullptr;
}

std::int64_t CompoundTag::getInt64(StringView name) const
{
    if (const auto *tag = getInt64Tag(name)) {
        return tag->data;
    }
    return 0;
}

const Int64Tag *CompoundTag::getInt64Tag(StringView name) const
{
    if (const auto *tag = get(name); tag) {
        if (tag->getId() == Type::Int64) {
            return static_cast<const Int64Tag *>(tag);
        }
    }
    return nullptr;
}

Int64Tag *CompoundTag::getInt64Tag(StringView name)
{
    if (auto *tag = get(name); tag) {
        if (tag->getId() == Type::Int64) {
            return static_cast<Int64Tag *>(tag);
        }
    }
    return nullptr;
}

std::int16_t CompoundTag::getShort(StringView name) const
{
    if (const auto *tag = getShortTag(name)) {
        return tag->data;
    }
    return 0;
}

const ShortTag *CompoundTag::getShortTag(StringView name) const
{
    if (const auto *tag = get(name); tag) {
        if (tag->getId() == Type::Short) {
            return static_cast<const ShortTag *>(tag);
        }
    }
    return nullptr;
}

const std::string &CompoundTag::getString(StringView key) const
{
    static std::string empty;
    if (const auto *tag = get(key); tag) {
        if (tag->getId() == Type::String) {
            return static_cast<const StringTag *>(tag)->data;
        }
    }
    return empty;
}

const CompoundTag *CompoundTag::getCompound(StringView key) const
{
    if (const auto *tag = get(key); tag) {
        if (tag->getId() == Type::Compound) {
            return static_cast<const CompoundTag *>(tag);
        }
    }
    return nullptr;
}

CompoundTag *CompoundTag::getCompound(StringView key)
{
    if (auto *tag = get(key); tag) {
        if (tag->getId() == Type::Compound) {
            return static_cast<CompoundTag *>(tag);
        }
    }
    return nullptr;
}

const ListTag *CompoundTag::getList(StringView name) const
{
    if (auto *tag = get(name); tag) {
        if (tag->getId() == Type::List) {
            return static_cast<const ListTag *>(tag);
        }
    }
    return nullptr;
}

ListTag *CompoundTag::getList(StringView name)
{
    if (auto *tag = get(name); tag) {
        if (tag->getId() == Type::List) {
            return static_cast<ListTag *>(tag);
        }
    }
    return nullptr;
}

bool CompoundTag::getBoolean(StringView key) const
{
    return getByte(key) != 0;
}

bool CompoundTag::contains(StringView key) const
{
    return get(key) != nullptr;
}

bool CompoundTag::contains(StringView key, Tag::Type type) const
{
    if (const auto *tag = get(key); tag) {
        return tag->getId() == type;
    }
    return false;
}

bool CompoundTag::isEmpty() const
{
    return tags_.empty();
}

void CompoundTag::deepCopy(const CompoundTag &other)
{
    tags_.clear();
    for (const auto &[key, value] : other.tags_) {
        tags_[key].emplace(std::move(*value.get()->copy()));
    }
}

std::unique_ptr<CompoundTag> CompoundTag::clone() const
{
    auto new_tag = std::make_unique<CompoundTag>();
    new_tag->deepCopy(*this);
    return new_tag;
}

bool CompoundTag::remove(StringView name)
{
    auto it = tags_.find(name);
    if (it == tags_.end()) {
        return false;
    }
    tags_.erase(it);
    return true;
}

TagMap::const_iterator CompoundTag::begin() const
{
    return tags_.begin();
}

TagMap::const_iterator CompoundTag::end() const
{
    return tags_.end();
}

std::size_t CompoundTag::size() const
{
    return tags_.size();
}
