

# File tag.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**nbt**](dir_777e494cd9d9b9aeedcd0556d83eeb02.md) **>** [**tag.h**](tag_8h.md)

[Go to the documentation of this file](tag_8h.md)


```C++
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

#include "endstone/nbt/array.h"
#include "endstone/nbt/compound.h"
#include "endstone/nbt/list.h"
#include "endstone/nbt/type.h"
#include "endstone/nbt/value.h"

namespace endstone {
using ByteTag = nbt::ValueTag<std::uint8_t>;
using ShortTag = nbt::ValueTag<std::int16_t>;
using IntTag = nbt::ValueTag<std::int32_t>;
using LongTag = nbt::ValueTag<std::int64_t>;
using FloatTag = nbt::ValueTag<float>;
using DoubleTag = nbt::ValueTag<double>;
using ByteArrayTag = nbt::ArrayTag<std::uint8_t>;
using StringTag = nbt::ValueTag<std::string>;
using IntArrayTag = nbt::ArrayTag<std::int32_t>;

namespace nbt {
class Tag {
public:
    using Storage = std::variant<std::monostate, ByteTag, ShortTag, IntTag, LongTag, FloatTag, DoubleTag, StringTag,
                                 ByteArrayTag, IntArrayTag, ListTag, CompoundTag>;

    Tag() noexcept : storage_(std::monostate()) {}
    explicit Tag(ByteTag v) : storage_(std::move(v)) {}
    explicit Tag(ShortTag v) : storage_(std::move(v)) {}
    explicit Tag(IntTag v) : storage_(std::move(v)) {}
    explicit Tag(LongTag v) : storage_(std::move(v)) {}
    explicit Tag(FloatTag v) : storage_(std::move(v)) {}
    explicit Tag(DoubleTag v) : storage_(std::move(v)) {}
    explicit Tag(StringTag v) : storage_(std::move(v)) {}
    explicit Tag(ByteArrayTag v) : storage_(std::move(v)) {}
    explicit Tag(IntArrayTag v) : storage_(std::move(v)) {}
    explicit Tag(ListTag v) : storage_(std::move(v)) {}
    explicit Tag(CompoundTag v) : storage_(std::move(v)) {}

    // kind / size (definitions not shown here)
    Type type() const noexcept;
    [[nodiscard]] std::size_t size() const noexcept;
    [[nodiscard]] bool empty() const noexcept;

    // self-shaping access (definitions not shown here)
    Tag &operator[](std::string_view key);
    Tag &operator[](std::size_t index);
    Tag &at(std::string_view key);
    const Tag &at(std::string_view key) const;
    Tag &at(std::size_t index);
    const Tag &at(std::size_t index) const;
    bool contains(std::string_view key) const noexcept;

    // getters (trait-driven)
    template <typename T>
    decltype(auto) get()
    {
        if (auto p = std::get_if<T>(&storage_)) {
            return *p;
        }
        throw std::runtime_error("Tag::get<T>() kind mismatch");
    }

    template <typename T>
    decltype(auto) get() const
    {
        if (auto p = std::get_if<T>(&storage_)) {
            return static_cast<const T &>(*p);
        }
        throw std::runtime_error("Tag::get<T>() kind mismatch");
    }

    template <typename T>
    auto get_if() noexcept
    {
        if (auto p = std::get_if<T>(&storage_)) {
            return p;
        }
        return nullptr;
    }

    template <typename T>
    const T *get_if() const noexcept
    {
        return std::get_if<T>(&storage_);
    }

    // equality
    friend bool operator==(const Tag &a, const Tag &b) noexcept
    {
        return a.storage_ == b.storage_;
    }
    friend bool operator!=(const Tag &a, const Tag &b) noexcept
    {
        return !(a == b);
    }

private:
    Storage storage_;
};
}  // namespace nbt

// --- ListTag ---
template <typename T>
ListTag::ListTag(std::initializer_list<T> init) : type_(T::TagType), elements_(init)
{
}

inline bool ListTag::empty() const noexcept
{
    return elements_.empty();
}

inline ListTag::size_type ListTag::size() const noexcept
{
    return elements_.size();
}

inline nbt::Type ListTag::type() const noexcept
{
    return type_;
}

inline ListTag::value_type &ListTag::at(size_type i)
{
    if (i >= elements_.size()) {
        throw std::out_of_range("ListTag::at");
    }
    return elements_.at(i);
}

inline const ListTag::value_type &ListTag::at(size_type i) const
{
    if (i >= elements_.size()) {
        throw std::out_of_range("ListTag::at");
    }
    return elements_.at(i);
}

inline ListTag::value_type &ListTag::operator[](size_type i)
{
    return elements_[i];
}

inline const ListTag::value_type &ListTag::operator[](size_type i) const
{
    return elements_[i];
}

inline void ListTag::clear() noexcept
{
    elements_.clear();
    type_ = nbt::Type::End;
}

inline ListTag::iterator ListTag::erase(const_iterator pos)
{
    return elements_.erase(pos);
}

inline ListTag::iterator ListTag::erase(const_iterator first, const_iterator last)
{
    return elements_.erase(first, last);
}

inline ListTag::iterator ListTag::begin() noexcept
{
    return elements_.begin();
}

inline ListTag::iterator ListTag::end() noexcept
{
    return elements_.end();
}

inline ListTag::const_iterator ListTag::begin() const noexcept
{
    return elements_.begin();
}

inline ListTag::const_iterator ListTag::end() const noexcept
{
    return elements_.end();
}

inline ListTag::const_iterator ListTag::cbegin() const noexcept
{
    return elements_.cbegin();
}

inline ListTag::const_iterator ListTag::cend() const noexcept
{
    return elements_.cend();
}

inline bool operator==(const ListTag &a, const ListTag &b) noexcept
{
    return a.type_ == b.type_ && a.elements_ == b.elements_;
}

inline bool operator!=(const ListTag &a, const ListTag &b) noexcept
{
    return !(a == b);
}
}  // namespace endstone
```


