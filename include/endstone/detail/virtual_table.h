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

#include <spdlog/spdlog.h>

#include "endstone/detail/cast.h"

namespace endstone::detail {

template <typename T>
class VirtualTable {
public:
    constexpr VirtualTable(std::uintptr_t *original, std::size_t size) : original_(original), size_(size){};

    [[nodiscard]] constexpr std::uintptr_t *begin() const
    {
        return original_;
    }

    [[nodiscard]] constexpr std::uintptr_t *end() const
    {
        return original_ + size_;
    }

    [[nodiscard]] constexpr std::uintptr_t at(size_t index) const
    {
        return original_[index];
    }

    [[nodiscard]] constexpr std::size_t size() const
    {
        return size_;
    }

    template <std::size_t Index, typename Return, typename... Arg>
    constexpr Return call(Arg &&...args) const
    {
        spdlog::info("{}", reinterpret_cast<void *>(at(Index)));
        auto func = reinterpret_cast<Return (*)(Arg...)>(at(Index));
        return func(std::forward<Arg>(args)...);
    }

private:
    std::uintptr_t *original_;
    std::size_t size_;
};

template <typename T>
class VirtualTableHook {
public:
    VirtualTableHook(T &target, std::size_t size)
        : original_(entt::locator<VirtualTable<T>>::value_or(*reinterpret_cast<std::uintptr_t **>(&target), size))
    {
        if (!mCopy) {
            mCopy = std::make_unique<std::uintptr_t[]>(size + TYPEINFO_SIZE);
            std::copy(original_.begin() - TYPEINFO_SIZE, original_.end(), mCopy.get());
            init();
        }
    }

    void hook(T &target)
    {
        *reinterpret_cast<std::uintptr_t **>(&target) = mCopy.get() + TYPEINFO_SIZE;
    }

    void reset(T &target)
    {
        *reinterpret_cast<std::uintptr_t **>(&target) = original_.begin();
    }

private:
    void init();

    template <std::size_t Index, typename Return, typename... Arg>
    void hook(Return (*fp)(Arg...))
    {
        mCopy[Index] = reinterpret_cast<std::uintptr_t>(fp_cast(fp));
    }

    inline static constexpr std::size_t TYPEINFO_SIZE = _WIN32_LINUX_(0, 2);
    inline static std::unique_ptr<std::uintptr_t[]> mCopy;
    VirtualTable<T> &original_;
};

}  // namespace endstone::detail
