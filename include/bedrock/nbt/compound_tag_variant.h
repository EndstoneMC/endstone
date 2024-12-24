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

#include <variant>

class CompoundTagVariant {
    using Variant = std::variant<EndTag, ByteTag, ShortTag, IntTag, Int64Tag, FloatTag, DoubleTag, ByteArrayTag,
                                 StringTag, ListTag, CompoundTag, IntArrayTag>;

public:
    template <typename T, typename... Args>
    T &emplace(Args &&...args)
    {
        tag_storage = T(std::forward<Args>(args)...);
        return std::get<T>(tag_storage);
    }

    Tag &emplace(Tag &&tag)
    {
        switch (tag.getId()) {
        case Tag::Type::Byte:
            return emplace<ByteTag>(static_cast<ByteTag &&>(tag));
        case Tag::Type::Short:
            return emplace<ShortTag>(static_cast<ShortTag &&>(tag));
        case Tag::Type::Int:
            return emplace<IntTag>(static_cast<IntTag &&>(tag));
        case Tag::Type::Int64:
            return emplace<Int64Tag>(static_cast<Int64Tag &&>(tag));
        case Tag::Type::Float:
            return emplace<FloatTag>(static_cast<FloatTag &&>(tag));
        case Tag::Type::Double:
            return emplace<DoubleTag>(static_cast<DoubleTag &&>(tag));
        case Tag::Type::ByteArray:
            return emplace<ByteArrayTag>(static_cast<ByteArrayTag &&>(tag));
        case Tag::Type::String:
            return emplace<StringTag>(static_cast<StringTag &&>(tag));
        case Tag::Type::List:
            return emplace<ListTag>(static_cast<ListTag &&>(tag));
        case Tag::Type::Compound:
            return emplace<CompoundTag>(static_cast<CompoundTag &&>(tag));
        case Tag::Type::IntArray:
            return emplace<IntArrayTag>(static_cast<IntArrayTag &&>(tag));
        case Tag::Type::End:
        default:
            return emplace<EndTag>();
        }
    }

    const Tag *operator->() const
    {
        return get();
    }

    Tag *operator->()
    {
        return get();
    }

    [[nodiscard]] const Tag *get() const
    {
        return std::visit([](auto &&arg) -> const Tag * { return &arg; }, tag_storage);
    }

    Tag *get()
    {
        return std::visit([](auto &&arg) -> Tag * { return &arg; }, tag_storage);
    }

    const Tag &operator*() const
    {
        return *get();
    }

    Tag &operator*()
    {
        return *get();
    }

    Variant tag_storage;
};
BEDROCK_STATIC_ASSERT_SIZE(CompoundTagVariant, 48, 48);
