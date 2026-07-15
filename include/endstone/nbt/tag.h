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

#include <bit>
#include <cstddef>
#include <cstdint>
#include <format>
#include <regex>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <variant>

#include "endstone/detail.h"
#include "endstone/nbt/array.h"
#include "endstone/nbt/compound.h"
#include "endstone/nbt/list.h"
#include "endstone/nbt/type.h"
#include "endstone/nbt/value.h"
#include "endstone/util/format.h"

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
/**
 * A single NBT tag that may hold any of the supported payload types.
 *
 * A Tag behaves as a tagged union over every NBT type (byte, short, int, long, float, double,
 * string, byte array, int array, list and compound). A default-constructed Tag is empty (the End
 * type). Indexing or inserting into an empty Tag promotes it to a ListTag or CompoundTag as
 * appropriate.
 */
class Tag {
public:
    using Storage = std::variant<std::monostate, ByteTag, ShortTag, IntTag, LongTag, FloatTag, DoubleTag, StringTag,
                                 ByteArrayTag, IntArrayTag, ListTag, CompoundTag>;

    Tag() noexcept : storage_(std::monostate()) {}
    Tag(const ByteTag &v) : storage_(v) {}
    Tag(ByteTag &&v) : storage_(std::move(v)) {}
    Tag(const ShortTag &v) : storage_(v) {}
    Tag(ShortTag &&v) : storage_(std::move(v)) {}
    Tag(const IntTag &v) : storage_(v) {}
    Tag(IntTag &&v) : storage_(std::move(v)) {}
    Tag(const LongTag &v) : storage_(v) {}
    Tag(LongTag &&v) : storage_(std::move(v)) {}
    Tag(const FloatTag &v) : storage_(v) {}
    Tag(FloatTag &&v) : storage_(std::move(v)) {}
    Tag(const DoubleTag &v) : storage_(v) {}
    Tag(DoubleTag &&v) : storage_(std::move(v)) {}
    Tag(const StringTag &v) : storage_(v) {}
    Tag(StringTag &&v) : storage_(std::move(v)) {}
    Tag(const ByteArrayTag &v) : storage_(v) {}
    Tag(ByteArrayTag &&v) : storage_(std::move(v)) {}
    Tag(const IntArrayTag &v) : storage_(v) {}
    Tag(IntArrayTag &&v) : storage_(std::move(v)) {}
    Tag(const ListTag &v) : storage_(v) {}
    Tag(ListTag &&v) : storage_(std::move(v)) {}
    Tag(const CompoundTag &v) : storage_(v) {}
    Tag(CompoundTag &&v) : storage_(std::move(v)) {}

    /**
     * Get the type of payload currently held by this tag.
     *
     * @return the tag type, or Type::End if the tag is empty
     */
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

    /**
     * Get the number of elements held by this tag.
     *
     * For a list, compound or array this is the element count; for a string it is the character
     * count; scalar tags report 1 and an empty tag reports 0.
     *
     * @return the element count
     */
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

    /**
     * Check whether this tag holds no elements.
     *
     * @return `true` if size() is 0
     */
    [[nodiscard]] bool empty() const noexcept { return size() == 0; }

    /**
     * Access the child tag with the given key, treating this tag as a compound.
     *
     * An empty tag is promoted to a CompoundTag, and a missing key is inserted with an empty value.
     *
     * @param key the key to access
     * @return a reference to the child tag
     * @throws std::runtime_error if this tag is not a compound
     */
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

    /**
     * Access the child tag at the given index, treating this tag as a list.
     *
     * An empty tag is promoted to a ListTag.
     *
     * @param index the index to access
     * @return a reference to the child tag
     * @throws std::runtime_error if this tag is not a list
     */
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

    /**
     * Access the child tag with the given key, treating this tag as a compound, with bounds checking.
     *
     * An empty tag is promoted to a CompoundTag.
     *
     * @param key the key to access
     * @return a reference to the child tag
     * @throws std::runtime_error if this tag is not a compound
     * @throws std::out_of_range if the key is not present
     */
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

    /**
     * Access the child tag with the given key, treating this tag as a compound, with bounds checking.
     *
     * @param key the key to access
     * @return a const reference to the child tag
     * @throws std::runtime_error if this tag is not a compound
     * @throws std::out_of_range if the key is not present
     */
    [[nodiscard]] const Tag &at(const std::string &key) const
    {
        const auto comp = std::get_if<CompoundTag>(&storage_);
        if (!comp) {
            throw std::runtime_error("Tag::at(key) const requires CompoundTag");
        }
        return comp->at(key);
    }

    /**
     * Access the child tag at the given index, treating this tag as a list, with bounds checking.
     *
     * An empty tag is promoted to a ListTag.
     *
     * @param index the index to access
     * @return a reference to the child tag
     * @throws std::runtime_error if this tag is not a list
     * @throws std::out_of_range if the index is out of range
     */
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

    /**
     * Access the child tag at the given index, treating this tag as a list, with bounds checking.
     *
     * @param index the index to access
     * @return a const reference to the child tag
     * @throws std::runtime_error if this tag is not a list
     * @throws std::out_of_range if the index is out of range
     */
    [[nodiscard]] const Tag &at(std::size_t index) const
    {
        const auto list = std::get_if<ListTag>(&storage_);
        if (!list) {
            throw std::runtime_error("Tag::at(index) requires ListTag");
        }
        return list->at(index);
    }

    /**
     * Check whether this tag is a compound containing the given key.
     *
     * @param key the key to look up
     * @return `true` if this tag is a compound and contains the key, `false` otherwise
     */
    [[nodiscard]] bool contains(const std::string &key) const noexcept
    {
        const auto comp = std::get_if<CompoundTag>(&storage_);
        if (!comp) {
            return false;
        }
        return comp->contains(key);
    }

    /**
     * Construct a child tag in place at the end of this list.
     *
     * An empty tag is promoted to a ListTag.
     *
     * @param args the arguments forwarded to the element constructor
     * @return a reference to this tag's underlying list
     * @throws std::runtime_error if this tag is not a list
     */
    template <class... Args>
    ListTag &emplace_back(Args &&...args)
    {
        if (std::holds_alternative<std::monostate>(storage_)) {
            storage_.emplace<ListTag>();
        }
        auto list = std::get_if<ListTag>(&storage_);
        if (!list) {
            throw std::runtime_error("Tag::emplace_back requires ListTag");
        }
        list->emplace_back(std::forward<Args>(args)...);
        return *list;
    }

    /**
     * Construct an entry in place in this compound.
     *
     * An empty tag is promoted to a CompoundTag.
     *
     * @param args the arguments forwarded to construct the key/value entry
     * @return a pair of an iterator to the entry and a bool that is `true` if a new entry was inserted
     * @throws std::runtime_error if this tag is not a compound
     */
    template <class... Args>
    std::pair<CompoundTag::iterator, bool> emplace(Args &&...args)
    {
        if (std::holds_alternative<std::monostate>(storage_)) {
            storage_.emplace<CompoundTag>();
        }
        auto comp = std::get_if<CompoundTag>(&storage_);
        if (!comp) {
            throw std::runtime_error("Tag::emplace requires CompoundTag");
        }
        return comp->emplace(std::forward<Args>(args)...);
    }

    /**
     * Get the payload as the given tag type.
     *
     * @tparam T the tag type to retrieve
     * @return a reference to the held value
     * @throws std::runtime_error if this tag does not currently hold a T
     */
    template <typename T>
    T &get()
    {
        if (auto p = std::get_if<T>(&storage_)) {
            return *p;
        }
        throw std::runtime_error("Tag::get<T>() kind mismatch");
    }

    /**
     * Get the payload as the given tag type.
     *
     * @tparam T the tag type to retrieve
     * @return a const reference to the held value
     * @throws std::runtime_error if this tag does not currently hold a T
     */
    template <typename T>
    const T &get() const
    {
        if (auto p = std::get_if<T>(&storage_)) {
            return *p;
        }
        throw std::runtime_error("Tag::get<T>() kind mismatch");
    }

    /**
     * Get a pointer to the payload if this tag holds the given type.
     *
     * @tparam T the tag type to retrieve
     * @return a pointer to the held value, or nullptr if this tag does not hold a T
     */
    template <typename T>
    T *get_if() noexcept
    {
        if (auto p = std::get_if<T>(&storage_)) {
            return p;
        }
        return nullptr;
    }

    /**
     * Get a pointer to the payload if this tag holds the given type.
     *
     * @tparam T the tag type to retrieve
     * @return a const pointer to the held value, or nullptr if this tag does not hold a T
     */
    template <typename T>
    const T *get_if() const noexcept
    {
        return std::get_if<T>(&storage_);
    }

    /**
     * Apply a visitor to the payload currently held by this tag.
     *
     * @tparam Fn the visitor type, callable with any of the tag payload types (including std::monostate)
     * @param visitor the visitor to invoke
     * @return whatever the visitor returns
     */
    template <typename Fn>
    decltype(auto) visit(Fn &&visitor) const &
    {
        return std::visit(visitor, storage_);
    }

    /**
     * Apply a visitor to the payload currently held by this tag.
     *
     * @tparam Fn the visitor type, callable with any of the tag payload types (including std::monostate)
     * @param visitor the visitor to invoke
     * @return whatever the visitor returns
     */
    template <typename Fn>
    decltype(auto) visit(Fn &&visitor) &
    {
        return std::visit(visitor, storage_);
    }

    /**
     * Serialize this tag to binary NBT with an empty root name.
     *
     * @param byte_order std::endian::little (Bedrock) or std::endian::big (Java).
     * @param network If `true`, use Bedrock network varint encoding.
     * @return Binary NBT data.
     * @see endstone::nbt::dump(), endstone::nbt::load()
     */
    [[nodiscard]] std::string dump(std::endian byte_order = std::endian::little, bool network = false) const;

    /**
     * Serialize this tag to binary NBT with a root name.
     *
     * @param name The root tag name.
     * @param byte_order std::endian::little (Bedrock) or std::endian::big (Java).
     * @param network If `true`, use Bedrock network varint encoding.
     * @return Binary NBT data.
     */
    [[nodiscard]] std::string dump(const std::string &name, std::endian byte_order = std::endian::little,
                                   bool network = false) const;

    friend bool operator==(const Tag &a, const Tag &b) noexcept { return a.storage_ == b.storage_; }

    friend bool operator!=(const Tag &a, const Tag &b) noexcept { return !(a == b); }

private:
    Storage storage_;
};
}  // namespace nbt

// --- ListTag ---
template <typename T>
    requires(!std::is_same_v<std::remove_cvref_t<T>, nbt::Tag>)
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
std::pair<CompoundTag::iterator, bool> CompoundTag::emplace(Args &&...args)
{
    return entries_.emplace(std::forward<Args>(args)...);
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

namespace nbt {
struct escape_view {
    std::string_view value;
};
inline escape_view escaped(std::string_view value)
{
    return {value};
}
}  // namespace nbt
}  // namespace endstone

template <>
struct std::formatter<endstone::nbt::Tag> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const endstone::nbt::Tag &tag, FormatContext &ctx) const -> format_context::iterator
    {
        return tag.visit([&](auto &&arg) {
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, std::monostate>) {
                return ctx.out();
            }
            else {
                return std::format_to(ctx.out(), "{}", arg);
            }
        });
    }
};

template <>
struct std::formatter<endstone::ByteTag> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const endstone::ByteTag &tag, FormatContext &ctx) const -> format_context::iterator
    {
        return std::format_to(ctx.out(), "{}b", tag.value());
    }
};

template <>
struct std::formatter<endstone::ShortTag> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const endstone::ShortTag &tag, FormatContext &ctx) const -> format_context::iterator
    {
        return std::format_to(ctx.out(), "{}s", tag.value());
    }
};

template <>
struct std::formatter<endstone::IntTag> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const endstone::IntTag &tag, FormatContext &ctx) const -> format_context::iterator
    {
        return std::format_to(ctx.out(), "{}", tag.value());
    }
};

template <>
struct std::formatter<endstone::LongTag> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const endstone::LongTag &tag, FormatContext &ctx) const -> format_context::iterator
    {
        return std::format_to(ctx.out(), "{}L", tag.value());
    }
};

template <>
struct std::formatter<endstone::FloatTag> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const endstone::FloatTag &tag, FormatContext &ctx) const -> format_context::iterator
    {
        std::string s = std::format("{:g}", tag.value());
        if (s.find('.') == std::string::npos && s.find('e') == std::string::npos) {
            s += ".0";
        }
        return std::format_to(ctx.out(), "{}f", s);
    }
};

template <>
struct std::formatter<endstone::DoubleTag> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const endstone::DoubleTag &tag, FormatContext &ctx) const -> format_context::iterator
    {
        std::string s = std::format("{:g}", tag.value());
        if (s.find('.') == std::string::npos && s.find('e') == std::string::npos) {
            s += ".0";
        }
        return std::format_to(ctx.out(), "{}d", s);
    }
};

template <>
struct std::formatter<endstone::ByteArrayTag> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const endstone::ByteArrayTag &tag, FormatContext &ctx) const -> format_context::iterator
    {
        auto out = ctx.out();
        out = std::format_to(out, "[B;");
        auto it = tag.begin();
        if (it != tag.end()) {
            out = std::format_to(out, "{}b", static_cast<int>(*it));
            ++it;
            while (it != tag.end()) {
                out = std::format_to(out, ",{}b", static_cast<int>(*it));
                ++it;
            }
        }
        out = std::format_to(out, "]");
        return out;
    }
};

template <>
struct std::formatter<endstone::StringTag> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const endstone::StringTag &tag, FormatContext &ctx) const -> format_context::iterator
    {
        return std::format_to(ctx.out(), "{}", endstone::nbt::escaped(tag.value()));
    }
};

template <>
struct std::formatter<endstone::IntArrayTag> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const endstone::IntArrayTag &tag, FormatContext &ctx) const -> format_context::iterator
    {
        return std::format_to(ctx.out(), "[I;{}]", endstone::detail::join(tag.begin(), tag.end(), ","));
    }
};

template <>
struct std::formatter<endstone::ListTag> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const endstone::ListTag &tag, FormatContext &ctx) const -> format_context::iterator
    {
        return std::format_to(ctx.out(), "[{}]", endstone::detail::join(tag.begin(), tag.end(), ","));
    }
};

template <>
struct std::formatter<endstone::CompoundTag::map_type::value_type> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const endstone::CompoundTag::map_type::value_type &pair,
                FormatContext &ctx) const -> format_context::iterator
    {
        return std::format_to(ctx.out(), "{}:{}", endstone::nbt::escaped(pair.first), pair.second);
    }
};

template <>
struct std::formatter<endstone::CompoundTag> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const endstone::CompoundTag &tag, FormatContext &ctx) const -> format_context::iterator
    {
        return std::format_to(ctx.out(), "{{{}}}", endstone::detail::join(tag.begin(), tag.end(), ","));
    }
};

template <>
struct std::formatter<endstone::nbt::escape_view> : std::formatter<std::string_view> {
    template <typename FormatContext>
    auto format(const endstone::nbt::escape_view &v, FormatContext &ctx) const -> format_context::iterator
    {
        static const std::regex simple_value("[A-Za-z0-9._+-]+");
        if (std::regex_match(v.value.begin(), v.value.end(), simple_value)) {
            return std::format_to(ctx.out(), "{}", v.value);
        }

        std::string out = " ";  // placeholder for chosen quote char
        char quote = 0;
        for (const char c : v.value) {
            switch (c) {
            case '\\':
                out.push_back('\\');
                out.push_back('\\');
                break;

            case '{':
                out += "{{";  // fmt escape
                break;

            case '}':
                out += "}}";  // fmt escape
                break;

            case '"':
            case '\'':
                if (quote == 0) {
                    // choose opposite quote
                    quote = (c == '"') ? '\'' : '"';
                }
                if (c == quote) {
                    out.push_back('\\');
                }
                out.push_back(c);
                break;

            default:
                out.push_back(c);
                break;
            }
        }

        if (quote == 0) {
            quote = '"';
        }

        out[0] = quote;
        out.push_back(quote);
        return std::format_to(ctx.out(), "{}", out);
    }
};

// ---- io.h must be included after all types are fully defined ----
#include "endstone/nbt/io.h"
