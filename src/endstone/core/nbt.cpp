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

#include "endstone/core/nbt.h"

namespace endstone::core::nbt {
endstone::nbt::Tag fromMinecraft(const ::Tag &tag)
{
    switch (tag.getId()) {
    case Tag::Type::Byte:
        return ByteTag{static_cast<const ::ByteTag &>(tag).data};
    case Tag::Type::Short:
        return ShortTag{static_cast<const ::ShortTag &>(tag).data};
    case Tag::Type::Int:
        return IntTag{static_cast<const ::IntTag &>(tag).data};
    case Tag::Type::Int64:
        return LongTag{static_cast<const Int64Tag &>(tag).data};
    case Tag::Type::Float:
        return FloatTag{static_cast<const ::FloatTag &>(tag).data};
    case Tag::Type::Double:
        return DoubleTag{static_cast<const ::DoubleTag &>(tag).data};
    case Tag::Type::ByteArray: {
        const auto &data = static_cast<const ::ByteArrayTag &>(tag).data;
        return ByteArrayTag{data.begin(), data.end()};
    }
    case Tag::Type::String:
        return StringTag{static_cast<const ::StringTag &>(tag).data};
    case Tag::Type::List: {
        const auto &list = static_cast<const ::ListTag &>(tag);
        ListTag converted;
        for (std::size_t i = 0; i < list.size(); ++i) {
            if (const auto *child = list.get(static_cast<int>(i))) {
                converted.emplace_back(fromMinecraft(*child));
            }
        }
        return converted;
    }
    case Tag::Type::Compound: {
        const auto &compound = static_cast<const ::CompoundTag &>(tag);
        CompoundTag converted;
        for (const auto &[key, value] : compound) {
            converted.insert_or_assign(key, fromMinecraft(*value.get()));
        }
        return converted;
    }
    case Tag::Type::IntArray: {
        const auto &data = static_cast<const ::IntArrayTag &>(tag).data;
        return IntArrayTag{data.begin(), data.end()};
    }
    default:
        return {};
    }
}

std::unique_ptr<::Tag> toMinecraft(const endstone::nbt::Tag &tag)
{
    switch (tag.type()) {
    case endstone::nbt::Type::End:
        return std::make_unique<EndTag>();
    case endstone::nbt::Type::Byte:
        return std::make_unique<::ByteTag>(tag.get<ByteTag>());
    case endstone::nbt::Type::Short:
        return std::make_unique<::ShortTag>(tag.get<ShortTag>());
    case endstone::nbt::Type::Int:
        return std::make_unique<::IntTag>(tag.get<IntTag>());
    case endstone::nbt::Type::Long:
        return std::make_unique<::Int64Tag>(tag.get<LongTag>());
    case endstone::nbt::Type::Float:
        return std::make_unique<::FloatTag>(tag.get<FloatTag>());
    case endstone::nbt::Type::Double:
        return std::make_unique<::DoubleTag>(tag.get<DoubleTag>());
    case endstone::nbt::Type::ByteArray: {
        auto &data = tag.get<ByteArrayTag>();
        return std::make_unique<::ByteArrayTag>(::ByteArrayTag::ArrayData{data.begin(), data.end()});
    }
    case endstone::nbt::Type::String:
        return std::make_unique<::StringTag>(tag.get<StringTag>());
    case endstone::nbt::Type::List: {
        const auto &list = tag.get<ListTag>();
        auto converted = std::make_unique<::ListTag>();
        for (const auto &value : list) {
            converted->add(toMinecraft(value));
        }
        return converted;
    }
    case endstone::nbt::Type::Compound: {
        const auto &compound = tag.get<CompoundTag>();
        auto converted = std::make_unique<::CompoundTag>();
        for (const auto &[key, value] : compound) {
            converted->put(key, toMinecraft(value));
        }
        return converted;
    }
    case endstone::nbt::Type::IntArray: {
        const auto &data = tag.get<IntArrayTag>();
        return std::make_unique<::IntArrayTag>(::IntArrayTag::ArrayData(data.begin(), data.end()));
    }
    default:
        return nullptr;
    }
}
}  // namespace endstone::core::nbt
