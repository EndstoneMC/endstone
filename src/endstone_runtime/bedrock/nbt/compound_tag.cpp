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

void CompoundTag::write(IDataOutput &output) const
{
    // TODO(nbt): fixme
    throw std::runtime_error("Not implemented");
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
    // TODO(nbt): fixme
    throw std::runtime_error("Not implemented");
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

std::uint8_t &CompoundTag::putByte(std::string name, std::uint8_t value)
{
    auto &it = tags_[name];
    it = ByteTag(value);
    return std::get<ByteTag>(it).data_;
}

void CompoundTag::putBoolean(std::string name, bool value)
{
    tags_[name] = ByteTag(value ? 1 : 0);
}

float &CompoundTag::putFloat(std::string name, float value)
{
    auto &it = tags_[name];
    it = FloatTag(value);
    return std::get<FloatTag>(it).data_;
}

std::int32_t &CompoundTag::putInt(std::string name, std::int32_t value)
{
    auto &it = tags_[name];
    it = IntTag(value);
    return std::get<IntTag>(it).data_;
}

std::int64_t &CompoundTag::putInt64(std::string name, std::int64_t value)
{
    auto &it = tags_[name];
    it = Int64Tag(value);
    return std::get<Int64Tag>(it).data_;
}

std::string &CompoundTag::putString(std::string name, std::string value)
{
    auto &it = tags_[name];
    it = StringTag(std::move(value));
    return std::get<StringTag>(it).data_;
}

CompoundTag &CompoundTag::putCompound(std::string name, CompoundTag value)
{
    auto &it = tags_[name];
    it = std::move(value);
    return std::get<CompoundTag>(it);
}
