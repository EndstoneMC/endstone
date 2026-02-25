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

#include <algorithm>
#include <array>
#include <cctype>
#include <string>
#include <cstdint>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <memory>
#include <stdexcept>
#include <string_view>
#include <utility>

#include <entt/entt.hpp>
#include <fmt/format.h>

#include "endstone/detail.h"

namespace endstone::runtime {
#include "bedrock_signatures.generated.h"

void *get_module_base();
std::string get_module_pathname();
void *get_executable_base();
std::string get_executable_pathname();
std::size_t get_executable_size();

namespace {
struct Pattern {
    std::vector<std::uint8_t> bytes;
    std::vector<bool> mask;
};

Pattern parse_pattern(std::string_view s)
{
    Pattern p;
    const char *data = s.data();
    std::size_t i = 0;
    while (i < s.size()) {
        while (i < s.size() && std::isspace(static_cast<unsigned char>(data[i]))) ++i;
        if (i >= s.size()) break;

        if (data[i] == '?') {
            p.bytes.push_back(0);
            p.mask.push_back(false);
            while (i < s.size() && !std::isspace(static_cast<unsigned char>(data[i]))) ++i;
            continue;
        }

        std::size_t j = i;
        while (j < s.size() && !std::isspace(static_cast<unsigned char>(data[j]))) ++j;
        int hex = 0;
        for (std::size_t k = i; k < j; ++k) {
            char c = data[k];
            hex <<= 4;
            if (std::isdigit(static_cast<unsigned char>(c))) hex |= (c - '0');
            else hex |= (10 + (std::toupper(static_cast<unsigned char>(c)) - 'A'));
        }
        p.bytes.push_back(static_cast<std::uint8_t>(hex));
        p.mask.push_back(true);
        i = j;
    }
    return p;
}

std::size_t scan(const Pattern &p)
{
    auto *base = static_cast<std::uint8_t *>(get_executable_base());
    auto size = get_executable_size();
    for (std::size_t i = 0; i + p.bytes.size() <= size; ++i) {
        bool ok = true;
        for (std::size_t j = 0; j < p.bytes.size(); ++j) {
            if (p.mask[j] && base[i + j] != p.bytes[j]) { ok = false; break; }
        }
        if (ok) return i;
    }
    throw std::runtime_error("signature not found");
}
}  // namespace

inline std::size_t get_symbol(std::string_view symbol)
{
    static std::unordered_map<std::string, std::size_t> cache;
    auto key = std::string(symbol);
    if (auto it = cache.find(key); it != cache.end()) return it->second;

    const SignatureItem *found = nullptr;
    for (const auto &item : signatures) {
        if (item.name == symbol) { found = &item; break; }
    }
    if (!found) throw std::runtime_error("symbol signature missing");

    auto pat = parse_pattern(found->pattern);
    auto match_off = scan(pat);

    std::size_t off = match_off;
    if (found->rip_relative) {
        auto *base = static_cast<std::uint8_t *>(get_executable_base());
        auto disp_ptr = base + match_off + static_cast<std::size_t>(found->rip_offset);
        auto disp = *reinterpret_cast<const std::int32_t *>(disp_ptr);
        auto target = (match_off + static_cast<std::size_t>(found->rip_offset) + 4) + static_cast<std::size_t>(disp);
        off = target;
    }
    off += static_cast<std::size_t>(found->extra);

    cache.emplace(key, off);
    return off;
}

template <typename Func>
inline void foreach_symbol(Func &&func)
{
    for (const auto &item : signatures) {
        auto off = get_symbol(item.name);
        std::invoke(std::forward<Func>(func), item.name, off);
    }
}

template <typename Func, typename... Args>
constexpr decltype(auto) invoke(const char *function, Func &&func, Args &&...args)
{
    (void)function;
    return std::invoke(func, std::forward<Args>(args)...);
}

#ifdef _WIN32
template <typename Class, typename... Args>
Class *(*get_ctor(const std::size_t Offset)) (Class *, Args...)
{
    auto *addr = static_cast<char *>(get_executable_base()) + Offset;
    return reinterpret_cast<Class *(*)(Class *, Args...)>(addr);
}
#elif __linux__
template <typename Class, typename... Args>
void (*get_ctor(const std::size_t Offset)) (Class *, Args...)
{
    auto *addr = static_cast<char *>(get_executable_base()) + Offset;
    return reinterpret_cast<void (*)(Class *, Args...)>(addr);
}
#endif
}  // namespace endstone::runtime

#define BEDROCK_CALL(fp, ...)                                                                         \
    endstone::runtime::invoke(                                                                        \
        __FUNCDNAME__,                                                                                \
        endstone::detail::fp_cast(fp, static_cast<char *>(endstone::runtime::get_executable_base()) + \
                                          endstone::runtime::get_symbol(__FUNCDNAME__)),              \
        ##__VA_ARGS__)

#define BEDROCK_VAR(type, name)                                                            \
    reinterpret_cast<type>(static_cast<char *>(endstone::runtime::get_executable_base()) + \
                           endstone::runtime::get_symbol(name));

#define BEDROCK_CTOR(type, ...) \
    endstone::runtime::get_ctor<type, ##__VA_ARGS__>(endstone::runtime::get_symbol(__FUNCDNAME__))
