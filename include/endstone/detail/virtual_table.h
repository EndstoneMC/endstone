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

#include <cstdint>
#include <functional>
#include <unordered_map>

#include <entt/entt.hpp>
#include <spdlog/spdlog.h>

#include "endstone/detail/cast.h"
#include "endstone/endstone.h"

namespace endstone::detail {

template <typename T>
class VirtualTable {
public:
    constexpr explicit VirtualTable(std::uintptr_t *original) : original_(original) {}

    [[nodiscard]] constexpr std::uintptr_t *begin() const
    {
        return original_;
    }

    [[nodiscard]] constexpr std::uintptr_t *end() const
    {
        return original_ + SIZE;
    }

    [[nodiscard]] constexpr std::uintptr_t at(size_t index) const
    {
        return original_[index];
    }

    [[nodiscard]] constexpr std::size_t size() const
    {
        return SIZE;
    }

    template <std::size_t Index, typename Return, std::size_t Offset = _WIN32_LINUX_(0, 1), typename... Arg>
    constexpr Return call(Arg &&...args) const
    {
        auto func = reinterpret_cast<Return (*)(Arg...)>(at(Index + Offset));
        return func(std::forward<Arg>(args)...);
    }

    static const std::size_t SIZE;

private:
    std::uintptr_t *original_;
};

template <typename T>
class VirtualTableHook {
public:
    explicit VirtualTableHook(T &target)
        : original_(entt::locator<VirtualTable<T>>::value_or(*reinterpret_cast<std::uintptr_t **>(&target)))
    {
        copy_ = std::make_unique<std::uintptr_t[]>(original_.size() + typeinfo_size);
        std::copy(original_.begin() - typeinfo_size, original_.end(), copy_.get());
        init();
    }

    void install(T &target)
    {
        *reinterpret_cast<std::uintptr_t **>(&target) = copy_.get() + typeinfo_size;
    }

    void uninstall(T &target)
    {
        *reinterpret_cast<std::uintptr_t **>(&target) = original_.begin();
    }

private:
    void init();

    template <std::size_t Index, typename... Arg, std::size_t Offset = _WIN32_LINUX_(0, 1), typename Return,
              typename Class>
    void hook(Return (Class::*fp)(Arg...))
    {
        copy_[Index + Offset + typeinfo_size] = reinterpret_cast<std::uintptr_t>(fp_cast(fp));
    }

    inline static constexpr std::size_t typeinfo_size = _WIN32_LINUX_(0, 2);
    std::unique_ptr<std::uintptr_t[]> copy_;
    VirtualTable<T> &original_;
};

}  // namespace endstone::detail
