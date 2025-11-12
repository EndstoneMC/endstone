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

#include <iostream>

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

    Type type() const noexcept
    {
        switch (storage_.index()) {
        case 0:
            return Type::End;
        case 1:
            return Type::Byte;
        case 2:
            return Type::Short;
        case 3:
            return Type::Int;
        case 4:
            return Type::Long;
        case 5:
            return Type::Float;
        case 6:
            return Type::Double;
        case 7:
            return Type::String;
        case 8:
            return Type::ByteArray;
        case 9:
            return Type::IntArray;
        case 10:
            return Type::List;
        case 11:
            return Type::Compound;
        default:
            return Type::End;
        }
    }

    [[nodiscard]] std::size_t size() const noexcept
    {
        return std::visit(
            [](auto &&arg) -> std::size_t {
                using T = std::decay_t<decltype(arg)>;
                if constexpr (std::is_same_v<T, std::monostate>) {
                    return 0;
                }
                else if constexpr (std::is_integral_v<typename T::value_type> ||
                                   std::is_floating_point_v<typename T::value_type>) {
                    return 1;
                }
                else if constexpr (std::is_same_v<T, StringTag>) {
                    return arg.value().size();
                }
                else {
                    return arg.size();
                }
            },
            storage_);
    }

    [[nodiscard]] bool empty() const noexcept
    {
        return size() == 0;
    }

    Tag &operator[](const std::string &key)
    {
        if (std::holds_alternative<std::monostate>(storage_)) {
            storage_.emplace<CompoundTag>();
        }
        const auto comp = std::get_if<CompoundTag>(&storage_);
        if (!comp) {
            throw std::runtime_error("Tag::operator[](key) requires CompoundTag");
        }
        return (*comp)[key];
    }

    Tag &operator[](std::size_t index)
    {
        if (std::holds_alternative<std::monostate>(storage_)) {
            storage_.emplace<ListTag>();
        }
        const auto list = std::get_if<ListTag>(&storage_);
        if (!list) {
            throw std::runtime_error("Tag::operator[](index) requires ListTag");
        }
        return (*list)[index];
    }

    Tag &at(const std::string &key)
    {
        if (std::holds_alternative<std::monostate>(storage_)) {
            storage_.emplace<CompoundTag>();
        }
        const auto comp = std::get_if<CompoundTag>(&storage_);
        if (!comp) {
            throw std::runtime_error("Tag::at(key) const requires CompoundTag");
        }
        return comp->at(key);
    }

    [[nodiscard]] const Tag &at(const std::string &key) const
    {
        const auto comp = std::get_if<CompoundTag>(&storage_);
        if (!comp) {
            throw std::runtime_error("Tag::at(key) const requires CompoundTag");
        }
        return comp->at(key);
    }

    Tag &at(std::size_t index)
    {
        if (std::holds_alternative<std::monostate>(storage_)) {
            storage_.emplace<ListTag>();
        }
        const auto list = std::get_if<ListTag>(&storage_);
        if (!list) {
            throw std::runtime_error("Tag::at(index) requires ListTag");
        }
        return list->at(index);
    }

    [[nodiscard]] const Tag &at(std::size_t index) const
    {
        const auto list = std::get_if<ListTag>(&storage_);
        if (!list) {
            throw std::runtime_error("Tag::at(index) requires ListTag");
        }
        return list->at(index);
    }

    [[nodiscard]] bool contains(const std::string &key) const noexcept
    {
        const auto comp = std::get_if<CompoundTag>(&storage_);
        if (!comp) {
            return false;
        }
        return comp->contains(key);
    }

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

    template <typename Fn>
    decltype(auto) visit(Fn &&visitor) const &
    {
        return std::visit(visitor, storage_);
    }

    template <typename Fn>
    decltype(auto) visit(Fn &&visitor) &
    {
        return std::visit(visitor, storage_);
    }

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
    requires(!std::is_same_v<T, nbt::Tag>)
ListTag::ListTag(std::initializer_list<T> init)
{
    for (const auto &item : init) {
        value_type v(item);
        const auto t = v.type();
        if (t == nbt::Type::End) {
            throw std::invalid_argument("ListTag cannot contain End tags.");
        }
        if (type_ == nbt::Type::End) {
            type_ = t;
        }
        else if (t != type_) {
            throw std::invalid_argument("ListTag elements must have the same type.");
        }
        elements_.emplace_back(v);
    }
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
    return elements_.at(i);
}

inline const ListTag::value_type &ListTag::at(size_type i) const
{
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

inline void ListTag::push_back(const value_type &v)
{
    const auto t = v.type();
    if (t == nbt::Type::End) {
        throw std::invalid_argument("ListTag cannot contain End tags.");
    }
    if (type_ == nbt::Type::End) {
        type_ = t;
    }
    else if (t != type_) {
        throw std::invalid_argument("ListTag elements must have the same type.");
    }
    elements_.push_back(v);
}

inline void ListTag::push_back(value_type &&v)
{
    const auto t = v.type();
    if (t == nbt::Type::End) {
        throw std::invalid_argument("ListTag cannot contain End tags.");
    }
    if (type_ == nbt::Type::End) {
        type_ = t;
    }
    else if (t != type_) {
        throw std::invalid_argument("ListTag elements must have the same type.");
    }
    elements_.push_back(v);
}

template <class... Args>
ListTag::value_type &ListTag::emplace_back(Args &&...args)
{
    value_type v(std::forward<Args>(args)...);
    const auto t = v.type();
    if (t == nbt::Type::End) {
        throw std::invalid_argument("ListTag cannot contain End tags.");
    }
    if (type_ == nbt::Type::End) {
        type_ = t;
    }
    else if (t != type_) {
        throw std::invalid_argument("ListTag elements must have the same type.");
    }
    elements_.emplace_back(v);
    return elements_.back();
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
    return a.elements_ == b.elements_;
}

inline bool operator!=(const ListTag &a, const ListTag &b) noexcept
{
    return !(a == b);
}

// --- CompoundTag
inline CompoundTag::CompoundTag(std::initializer_list<std::pair<std::string, nbt::Tag>> init)
    : entries_(init.begin(), init.end())
{
}

inline bool CompoundTag::empty() const noexcept
{
    return entries_.empty();
}

inline CompoundTag::size_type CompoundTag::size() const noexcept
{
    return entries_.size();
}

inline CompoundTag::value_type &CompoundTag::at(const key_type &key)
{
    return entries_.at(key);
}

inline const CompoundTag::value_type &CompoundTag::at(key_type key) const
{
    return entries_.at(key);
}

inline CompoundTag::value_type &CompoundTag::operator[](const key_type &key)
{
    return entries_[key];
}

inline bool CompoundTag::contains(const key_type &key) const noexcept
{
    return entries_.contains(key);
}

inline void CompoundTag::clear() noexcept
{
    entries_.clear();
}

template <class P>
std::pair<CompoundTag::iterator, bool> CompoundTag::insert(P &&v)
{
    return entries_.insert(std::forward<P>(v));
}

template <class... Args>
std::pair<CompoundTag::iterator, bool> CompoundTag::try_emplace(const key_type &key, Args &&...args)
{
    return entries_.try_emplace(key, std::forward<Args>(args)...);
}

template <class M>
std::pair<CompoundTag::iterator, bool> CompoundTag::insert_or_assign(const key_type &key, M &&obj)
{
    return entries_.insert_or_assign(key, std::forward<M>(obj));
}

inline CompoundTag::iterator CompoundTag::erase(const_iterator pos)
{
    return entries_.erase(pos);
}

inline CompoundTag::size_type CompoundTag::erase(const key_type &key)
{
    return entries_.erase(key);
}

inline CompoundTag::iterator CompoundTag::erase(const_iterator first, const_iterator last)
{
    return entries_.erase(first, last);
}

inline void CompoundTag::swap(CompoundTag &other) noexcept
{
    entries_.swap(other.entries_);
}

inline void CompoundTag::merge(CompoundTag &source)
{
    for (const auto &[k, v] : source.entries_) {
        entries_.try_emplace(k, v);
    }
}

inline void CompoundTag::merge(CompoundTag &&source)
{
    for (const auto &[k, v] : source.entries_) {
        entries_.try_emplace(k, std::move(v));
    }
}

inline CompoundTag::iterator CompoundTag::begin() noexcept
{
    return entries_.begin();
}

inline CompoundTag::iterator CompoundTag::end() noexcept
{
    return entries_.end();
}

inline CompoundTag::const_iterator CompoundTag::begin() const noexcept
{
    return entries_.begin();
}

inline CompoundTag::const_iterator CompoundTag::end() const noexcept
{
    return entries_.end();
}

inline CompoundTag::const_iterator CompoundTag::cbegin() const noexcept
{
    return entries_.cbegin();
}

inline CompoundTag::const_iterator CompoundTag::cend() const noexcept
{
    return entries_.cend();
}

inline bool operator==(const CompoundTag &a, const CompoundTag &b) noexcept
{
    return a.entries_ == b.entries_;
}

inline bool operator!=(const CompoundTag &a, const CompoundTag &b) noexcept
{
    return !(a == b);
}
}  // namespace endstone

template <>
struct fmt::formatter<endstone::nbt::Tag> : formatter<string_view> {
    template <typename FormatContext>
    auto format(const endstone::nbt::Tag &tag, FormatContext &ctx) const -> format_context::iterator
    {
        return tag.visit([&](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                return ctx.out();
            }
            else {
                return fmt::format_to(ctx.out(), "{}", arg);
            }
        });
    }
};
