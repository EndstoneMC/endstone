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

#include <utility>

#include "bedrock/nbt/nbt_io.h"

void CompoundTag::write(IDataOutput &output) const
{
    for (const auto &[key, value] : tags_) {
        NbtIo::writeNamedTag(key, *value.get(), output);
    }
    output.writeByte(static_cast<std::uint8_t>(Tag::Type::End));
}

Bedrock::Result<void> CompoundTag::load(IDataInput &input)
{
    // TODO(nbt): fixme
    throw std::runtime_error("Not implemented");
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
    return std::all_of(tags_.begin(), tags_.end(), [&](const auto &kv) {
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

std::unique_ptr<CompoundTag> CompoundTag::clone() const
{
    auto new_tag = std::make_unique<CompoundTag>();
    new_tag->deepCopy(*this);
    return new_tag;
}

void CompoundTag::deepCopy(const CompoundTag &other)
{
    tags_.clear();
    for (const auto &[key, value] : other.tags_) {
        tags_[key].emplace(std::move(*value.get()->copy()));
    }
}

bool CompoundTag::contains(std::string_view key) const
{
    return get(key) != nullptr;
}

bool CompoundTag::contains(std::string_view key, Tag::Type type) const
{
    if (const auto *tag = get(key); tag) {
        return tag->getId() == type;
    }
    return false;
}

const Tag *CompoundTag::get(std::string_view key) const
{
    auto it = tags_.find(key);
    if (it != tags_.end()) {
        return it->second.get();
    }
    return nullptr;
}

const std::string &CompoundTag::getString(std::string_view key) const
{
    static std::string empty;
    if (const auto *tag = get(key); tag) {
        if (tag->getId() == Tag::Type::String) {
            return static_cast<const StringTag *>(tag)->data;
        }
    }
    return empty;
}

const CompoundTag *CompoundTag::getCompound(std::string_view key) const
{
    if (const auto *tag = get(key); tag) {
        if (tag->getId() == Tag::Type::Compound) {
            return static_cast<const CompoundTag *>(tag);
        }
    }
    return nullptr;
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

void CompoundTag::putBoolean(std::string name, bool value)
{
    tags_[name].emplace(ByteTag(value ? 1 : 0));
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

const Tag *CompoundTagVariant::get() const
{
    return std::visit([](auto &&arg) -> const Tag * { return &arg; }, tag_storage_);
}

Tag &CompoundTagVariant::emplace(Tag &&tag)
{
    switch (tag.getId()) {
    case Tag::Type::Byte:
        tag_storage_ = static_cast<ByteTag &&>(tag);
        return std::get<ByteTag>(tag_storage_);
    case Tag::Type::Short:
        tag_storage_ = static_cast<ShortTag &&>(tag);
        return std::get<ShortTag>(tag_storage_);
    case Tag::Type::Int:
        tag_storage_ = static_cast<IntTag &&>(tag);
        return std::get<IntTag>(tag_storage_);
    case Tag::Type::Int64:
        tag_storage_ = static_cast<Int64Tag &&>(tag);
        return std::get<Int64Tag>(tag_storage_);
    case Tag::Type::Float:
        tag_storage_ = static_cast<FloatTag &&>(tag);
        return std::get<FloatTag>(tag_storage_);
    case Tag::Type::Double:
        tag_storage_ = static_cast<DoubleTag &&>(tag);
        return std::get<DoubleTag>(tag_storage_);
    case Tag::Type::ByteArray:
        tag_storage_ = static_cast<ByteArrayTag &&>(tag);
        return std::get<ByteArrayTag>(tag_storage_);
    case Tag::Type::String:
        tag_storage_ = static_cast<StringTag &&>(tag);
        return std::get<StringTag>(tag_storage_);
    case Tag::Type::List:
        tag_storage_ = static_cast<ListTag &&>(tag);
        return std::get<ListTag>(tag_storage_);
    case Tag::Type::Compound:
        tag_storage_ = static_cast<CompoundTag &&>(tag);
        return std::get<CompoundTag>(tag_storage_);
    case Tag::Type::IntArray:
        tag_storage_ = static_cast<IntArrayTag &&>(tag);
        return std::get<IntArrayTag>(tag_storage_);
    case Tag::Type::End:
    default:
        tag_storage_ = static_cast<EndTag &&>(tag);
        return std::get<EndTag>(tag_storage_);
    }
}
