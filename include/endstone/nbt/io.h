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
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>

#include "endstone/nbt/tag.h"

namespace endstone::nbt {

namespace detail {

// ---- raw byte helpers ----

inline void write_bytes(std::string &out, const void *data, std::size_t n)
{
    out.append(reinterpret_cast<const char *>(data), n);
}

inline void read_bytes(std::string_view &in, void *dst, std::size_t n)
{
    if (in.size() < n) {
        throw std::runtime_error("nbt::load: unexpected end of data");
    }
    std::memcpy(dst, in.data(), n);
    in.remove_prefix(n);
}

// ---- endian-aware numeric I/O ----

template <typename T>
inline void write_value(std::string &out, T value, std::endian byte_order)
{
    static_assert(std::is_arithmetic_v<T>);
    if constexpr (sizeof(T) == 1) {
        out.push_back(static_cast<char>(value));
        return;
    }

    if (byte_order == std::endian::big) {
        if constexpr (std::endian::native == std::endian::little) {
            char buf[sizeof(T)];
            std::memcpy(buf, &value, sizeof(T));
            for (std::size_t i = 0; i < sizeof(T) / 2; ++i) {
                std::swap(buf[i], buf[sizeof(T) - 1 - i]);
            }
            write_bytes(out, buf, sizeof(T));
        }
        else {
            write_bytes(out, &value, sizeof(T));
        }
    }
    else {
        if constexpr (std::endian::native == std::endian::little) {
            write_bytes(out, &value, sizeof(T));
        }
        else {
            char buf[sizeof(T)];
            std::memcpy(buf, &value, sizeof(T));
            for (std::size_t i = 0; i < sizeof(T) / 2; ++i) {
                std::swap(buf[i], buf[sizeof(T) - 1 - i]);
            }
            write_bytes(out, buf, sizeof(T));
        }
    }
}

template <typename T>
inline T read_value(std::string_view &in, std::endian byte_order)
{
    static_assert(std::is_arithmetic_v<T>);
    T value{};
    if constexpr (sizeof(T) == 1) {
        if (in.empty()) {
            throw std::runtime_error("nbt::load: unexpected end of data");
        }
        value = static_cast<T>(static_cast<std::uint8_t>(in.front()));
        in.remove_prefix(1);
        return value;
    }
    else {
        char buf[sizeof(T)];
        read_bytes(in, buf, sizeof(T));
        if (byte_order == std::endian::big) {
            if constexpr (std::endian::native == std::endian::little) {
                for (std::size_t i = 0; i < sizeof(T) / 2; ++i) {
                    std::swap(buf[i], buf[sizeof(T) - 1 - i]);
                }
            }
        }
        else {
            if constexpr (std::endian::native == std::endian::big) {
                for (std::size_t i = 0; i < sizeof(T) / 2; ++i) {
                    std::swap(buf[i], buf[sizeof(T) - 1 - i]);
                }
            }
        }
        std::memcpy(&value, buf, sizeof(T));
        return value;
    }
}

// ---- unsigned varint (LEB128) ----

inline void write_varint(std::string &out, std::uint32_t value)
{
    while (value > 0x7F) {
        out.push_back(static_cast<char>((value & 0x7F) | 0x80));
        value >>= 7;
    }
    out.push_back(static_cast<char>(value));
}

inline std::uint32_t read_varint(std::string_view &in)
{
    std::uint32_t result = 0;
    int shift = 0;
    for (;;) {
        if (in.empty()) {
            throw std::runtime_error("nbt::load: unexpected end of varint");
        }
        auto byte = static_cast<std::uint8_t>(in.front());
        in.remove_prefix(1);
        result |= static_cast<std::uint32_t>(byte & 0x7F) << shift;
        if ((byte & 0x80) == 0) {
            break;
        }
        shift += 7;
        if (shift >= 35) {
            throw std::runtime_error("nbt::load: varint too large");
        }
    }
    return result;
}

// ---- zigzag encoding for signed ints ----

inline std::uint32_t zigzag_encode32(std::int32_t value)
{
    return static_cast<std::uint32_t>((value << 1) ^ (value >> 31));
}

inline std::int32_t zigzag_decode32(std::uint32_t value)
{
    return static_cast<std::int32_t>((value >> 1) ^ -(value & 1));
}

inline void write_zigzag32(std::string &out, std::int32_t value)
{
    write_varint(out, zigzag_encode32(value));
}

inline std::int32_t read_zigzag32(std::string_view &in)
{
    return zigzag_decode32(read_varint(in));
}

// 64-bit varint/zigzag

inline void write_varint64(std::string &out, std::uint64_t value)
{
    while (value > 0x7F) {
        out.push_back(static_cast<char>((value & 0x7F) | 0x80));
        value >>= 7;
    }
    out.push_back(static_cast<char>(value));
}

inline std::uint64_t read_varint64(std::string_view &in)
{
    std::uint64_t result = 0;
    int shift = 0;
    for (;;) {
        if (in.empty()) {
            throw std::runtime_error("nbt::load: unexpected end of varint64");
        }
        auto byte = static_cast<std::uint8_t>(in.front());
        in.remove_prefix(1);
        result |= static_cast<std::uint64_t>(byte & 0x7F) << shift;
        if ((byte & 0x80) == 0) {
            break;
        }
        shift += 7;
        if (shift >= 70) {
            throw std::runtime_error("nbt::load: varint64 too large");
        }
    }
    return result;
}

inline std::uint64_t zigzag_encode64(std::int64_t value)
{
    return static_cast<std::uint64_t>((value << 1) ^ (value >> 63));
}

inline std::int64_t zigzag_decode64(std::uint64_t value)
{
    return static_cast<std::int64_t>((value >> 1) ^ -(value & 1));
}

inline void write_zigzag64(std::string &out, std::int64_t value)
{
    write_varint64(out, zigzag_encode64(value));
}

inline std::int64_t read_zigzag64(std::string_view &in)
{
    return zigzag_decode64(read_varint64(in));
}

// ---- string I/O ----

inline void write_string(std::string &out, const std::string &str, std::endian byte_order, bool network)
{
    if (network) {
        write_varint(out, static_cast<std::uint32_t>(str.size()));
    }
    else {
        write_value<std::uint16_t>(out, static_cast<std::uint16_t>(str.size()), byte_order);
    }
    write_bytes(out, str.data(), str.size());
}

inline std::string read_string(std::string_view &in, std::endian byte_order, bool network)
{
    std::uint32_t len;
    if (network) {
        len = read_varint(in);
    }
    else {
        len = read_value<std::uint16_t>(in, byte_order);
    }
    if (in.size() < len) {
        throw std::runtime_error("nbt::load: unexpected end of string data");
    }
    std::string result(in.data(), len);
    in.remove_prefix(len);
    return result;
}

// ---- length I/O (int32 or zigzag varint depending on network flag) ----

inline void write_length(std::string &out, std::int32_t len, std::endian byte_order, bool network)
{
    if (network) {
        write_zigzag32(out, len);
    }
    else {
        write_value<std::int32_t>(out, len, byte_order);
    }
}

inline std::int32_t read_length(std::string_view &in, std::endian byte_order, bool network)
{
    if (network) {
        return read_zigzag32(in);
    }
    return read_value<std::int32_t>(in, byte_order);
}

// ---- payload write (recursive, visitor-based) ----

inline void write_tag_payload(std::string &out, const Tag &tag, std::endian byte_order, bool network)
{
    tag.visit([&](auto &&arg) {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, std::monostate>) {
            // End tag has no payload
        }
        else if constexpr (std::is_same_v<T, endstone::ByteTag>) {
            write_value<std::uint8_t>(out, arg.value(), byte_order);
        }
        else if constexpr (std::is_same_v<T, endstone::ShortTag>) {
            write_value<std::int16_t>(out, arg.value(), byte_order);
        }
        else if constexpr (std::is_same_v<T, endstone::IntTag>) {
            if (network) {
                write_zigzag32(out, arg.value());
            }
            else {
                write_value<std::int32_t>(out, arg.value(), byte_order);
            }
        }
        else if constexpr (std::is_same_v<T, endstone::LongTag>) {
            if (network) {
                write_zigzag64(out, arg.value());
            }
            else {
                write_value<std::int64_t>(out, arg.value(), byte_order);
            }
        }
        else if constexpr (std::is_same_v<T, endstone::FloatTag>) {
            write_value<float>(out, arg.value(), byte_order);
        }
        else if constexpr (std::is_same_v<T, endstone::DoubleTag>) {
            write_value<double>(out, arg.value(), byte_order);
        }
        else if constexpr (std::is_same_v<T, endstone::StringTag>) {
            write_string(out, arg.value(), byte_order, network);
        }
        else if constexpr (std::is_same_v<T, endstone::ByteArrayTag>) {
            write_length(out, static_cast<std::int32_t>(arg.size()), byte_order, network);
            write_bytes(out, arg.data(), arg.size());
        }
        else if constexpr (std::is_same_v<T, endstone::IntArrayTag>) {
            write_length(out, static_cast<std::int32_t>(arg.size()), byte_order, network);
            if (network) {
                for (std::size_t i = 0; i < arg.size(); ++i) {
                    write_zigzag32(out, arg[i]);
                }
            }
            else {
                for (std::size_t i = 0; i < arg.size(); ++i) {
                    write_value<std::int32_t>(out, arg[i], byte_order);
                }
            }
        }
        else if constexpr (std::is_same_v<T, endstone::ListTag>) {
            out.push_back(static_cast<char>(static_cast<std::uint8_t>(arg.type())));
            write_length(out, static_cast<std::int32_t>(arg.size()), byte_order, network);
            for (const auto &elem : arg) {
                write_tag_payload(out, elem, byte_order, network);
            }
        }
        else if constexpr (std::is_same_v<T, endstone::CompoundTag>) {
            for (const auto &[key, value] : arg) {
                out.push_back(static_cast<char>(static_cast<std::uint8_t>(value.type())));
                write_string(out, key, byte_order, network);
                write_tag_payload(out, value, byte_order, network);
            }
            out.push_back(static_cast<char>(0));  // End tag
        }
    });
}

// ---- payload read (recursive) ----

inline Tag read_tag_payload(std::string_view &in, Type type, std::endian byte_order, bool network)
{
    switch (type) {
    case Type::End:
        return Tag{};
    case Type::Byte:
        return Tag{endstone::ByteTag{read_value<std::uint8_t>(in, byte_order)}};
    case Type::Short:
        return Tag{endstone::ShortTag{read_value<std::int16_t>(in, byte_order)}};
    case Type::Int:
        if (network) {
            return Tag{endstone::IntTag{read_zigzag32(in)}};
        }
        return Tag{endstone::IntTag{read_value<std::int32_t>(in, byte_order)}};
    case Type::Long:
        if (network) {
            return Tag{endstone::LongTag{read_zigzag64(in)}};
        }
        return Tag{endstone::LongTag{read_value<std::int64_t>(in, byte_order)}};
    case Type::Float:
        return Tag{endstone::FloatTag{read_value<float>(in, byte_order)}};
    case Type::Double:
        return Tag{endstone::DoubleTag{read_value<double>(in, byte_order)}};
    case Type::String:
        return Tag{endstone::StringTag{read_string(in, byte_order, network)}};
    case Type::ByteArray: {
        auto len = read_length(in, byte_order, network);
        if (len < 0) {
            throw std::runtime_error("nbt::load: negative ByteArray length");
        }
        endstone::ByteArrayTag::storage_type vec(static_cast<std::size_t>(len));
        if (len > 0) {
            read_bytes(in, vec.data(), static_cast<std::size_t>(len));
        }
        return Tag{endstone::ByteArrayTag{std::move(vec)}};
    }
    case Type::IntArray: {
        auto len = read_length(in, byte_order, network);
        if (len < 0) {
            throw std::runtime_error("nbt::load: negative IntArray length");
        }
        endstone::IntArrayTag arr;
        for (std::int32_t i = 0; i < len; ++i) {
            if (network) {
                arr.push_back(read_zigzag32(in));
            }
            else {
                arr.push_back(read_value<std::int32_t>(in, byte_order));
            }
        }
        return Tag{std::move(arr)};
    }
    case Type::List: {
        auto elem_type = static_cast<Type>(read_value<std::uint8_t>(in, byte_order));
        auto len = read_length(in, byte_order, network);
        if (len < 0) {
            throw std::runtime_error("nbt::load: negative List length");
        }
        endstone::ListTag lt;
        for (std::int32_t i = 0; i < len; ++i) {
            lt.emplace_back(read_tag_payload(in, elem_type, byte_order, network));
        }
        return Tag{std::move(lt)};
    }
    case Type::Compound: {
        endstone::CompoundTag ct;
        for (;;) {
            auto entry_type = static_cast<Type>(read_value<std::uint8_t>(in, byte_order));
            if (entry_type == Type::End) {
                break;
            }
            auto key = read_string(in, byte_order, network);
            ct.insert_or_assign(key, read_tag_payload(in, entry_type, byte_order, network));
        }
        return Tag{std::move(ct)};
    }
    default:
        throw std::runtime_error("nbt::load: unknown tag type " +
                                 std::to_string(static_cast<int>(type)));
    }
}

}  // namespace detail

// ---- dump: serialize tag without name ----

inline std::string dump(const Tag &tag, std::endian byte_order = std::endian::little, bool network = false)
{
    std::string out;
    out.push_back(static_cast<char>(static_cast<std::uint8_t>(tag.type())));
    detail::write_tag_payload(out, tag, byte_order, network);
    return out;
}

// ---- dump: serialize tag with name ----

inline std::string dump(const Tag &tag, const std::string &name, std::endian byte_order = std::endian::little,
                        bool network = false)
{
    std::string out;
    out.push_back(static_cast<char>(static_cast<std::uint8_t>(tag.type())));
    detail::write_string(out, name, byte_order, network);
    detail::write_tag_payload(out, tag, byte_order, network);
    return out;
}

// ---- load: deserialize and populate name ----

inline Tag load(std::string_view data, std::string &name, std::endian byte_order = std::endian::little,
                bool network = false)
{
    auto type = static_cast<Type>(detail::read_value<std::uint8_t>(data, byte_order));
    if (type == Type::End) {
        name.clear();
        return Tag{};
    }
    name = detail::read_string(data, byte_order, network);
    return detail::read_tag_payload(data, type, byte_order, network);
}

// ---- load: deserialize, discarding name ----

inline Tag load(std::string_view data, std::endian byte_order = std::endian::little, bool network = false)
{
    std::string name;
    return load(data, name, byte_order, network);
}

// ---- Tag::dump() inline definitions ----

inline std::string Tag::dump(std::endian byte_order, bool network) const
{
    return nbt::dump(*this, byte_order, network);
}

inline std::string Tag::dump(const std::string &name, std::endian byte_order, bool network) const
{
    return nbt::dump(*this, name, byte_order, network);
}

}  // namespace endstone::nbt
