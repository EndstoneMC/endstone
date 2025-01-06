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

#include <memory>
#include <stdexcept>
#include <string_view>

#include "cast.h"

namespace endstone::runtime {

template <typename Key, typename Value, std::size_t Size>
class SymbolMap {
public:
    constexpr explicit SymbolMap(std::array<std::pair<Key, Value>, Size> data) : data_(data) {}
    constexpr Value at(const Key &key) const
    {
        for (const auto &pair : data_) {
            if (pair.first == key) {
                return pair.second;
            }
        }
        throw std::out_of_range("Key not found in ConstexprMap");
    }

private:
    std::array<std::pair<Key, Value>, Size> data_;
};

constexpr void *get_symbol_addr(std::string_view name);
}  // namespace endstone::runtime

#define ENDSTONE_SYMCALL(fp, ...) \
    std::invoke(endstone::runtime::fp_cast(fp, endstone::runtime::get_symbol_addr(__FUNCDNAME__)), ##__VA_ARGS__)

namespace endstone::runtime {
#ifdef _WIN32
template <typename Class, typename... Args>
Class *(*get_ctor(std::unique_ptr<Class> (*)(Args...), std::string_view name))(Class *, Args...)
{
    auto *original = get_symbol_addr(name);
    return reinterpret_cast<Class *(*)(Class *, Args...)>(original);
}
#elif __linux__
template <typename Class, typename... Args>
void (*get_ctor(std::unique_ptr<Class> (*)(Args...), std::string_view name))(Class *, Args...)
{
    auto *original = get_symbol_addr(name);
    return reinterpret_cast<void (*)(Class *, Args...)>(original);
}
#endif
}  // namespace endstone::runtime

#define ENDSTONE_FACTORY_DECLARE(type, ...) static std::unique_ptr<type> create(__VA_ARGS__);
#ifdef _WIN32
#define ENDSTONE_FACTORY_PREFIX_TARGET(type)      ENDSTONE_TOSTRING(type) "@@@std@@@std@"
#define ENDSTONE_FACTORY_PREFIX_REPLACEMENT(type) "??0" ENDSTONE_TOSTRING(type) "@@QEAA"
#elif __linux__
#define ENDSTONE_FACTORY_PREFIX_TARGET(type) ENDSTONE_TOSTRING(type) "6create"
#define ENDSTONE_FACTORY_PREFIX_REPLACEMENT(type) \
    "_ZN" + std::to_string(std::strlen(ENDSTONE_TOSTRING(type))) + ENDSTONE_TOSTRING(type) "C2"
#endif

#define ENDSTONE_FACTORY_IMPLEMENT(type, ...) ENDSTONE_FACTORY_IMPLEMENT_OVERLOAD(type, &type::create, ##__VA_ARGS__)

#define ENDSTONE_FACTORY_IMPLEMENT_OVERLOAD(type, fp, ...)                        \
    {                                                                             \
        static std::string func_decorated_name = __FUNCDNAME__;                   \
        static std::string ctor_decorated_name = []() {                           \
            std::string prefix_target = ENDSTONE_FACTORY_PREFIX_TARGET(type);     \
            std::string name = ENDSTONE_FACTORY_PREFIX_REPLACEMENT(type);         \
            std::size_t pos = func_decorated_name.find(prefix_target);            \
            pos += std::strlen(prefix_target.c_str());                            \
            name += func_decorated_name.substr(pos);                              \
            pos = name.find("BV1@@Z");                                            \
            if (pos != std::string::npos) {                                       \
                name.replace(pos, 6, "BV0@@Z");                                   \
            }                                                                     \
            return name;                                                          \
        }();                                                                      \
        auto *obj = reinterpret_cast<type *>(new char[sizeof(type)]);             \
        endstone::runtime::get_ctor(fp, ctor_decorated_name)(obj, ##__VA_ARGS__); \
        return std::unique_ptr<type>(obj);                                        \
    }
