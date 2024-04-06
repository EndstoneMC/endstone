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
    VirtualTable(T &target, std::size_t size) : VirtualTable(*reinterpret_cast<std::uintptr_t **>(&target), size) {}

    VirtualTable(std::uintptr_t *original, std::size_t size) : original_(original), size_(size)
    {
        copy_ = std::make_unique<std::uintptr_t[]>(size + type_info_size);
        std::copy(original_ - type_info_size, original_ + size, copy_.get());
    };

    [[nodiscard]] std::uintptr_t *original() const
    {
        return original_;
    }

    [[nodiscard]] std::uintptr_t *copy() const
    {
        return copy_.get() + type_info_size;
    }

    [[nodiscard]] std::size_t size() const
    {
        return size_;
    }

    void swap(T &target)
    {
        *reinterpret_cast<std::uintptr_t **>(&target) = copy();
    }

    void reset(T &target)
    {
        *reinterpret_cast<std::uintptr_t **>(&target) = original();
    }

    template <std::size_t index, typename Return, typename Class, typename... Arg>
    void hook(Return (Class::*fp)(Arg...))
    {
        hook<index>(fp_cast(fp));
    }

    template <std::size_t index, typename Return, typename Class, typename... Arg>
    Return callOriginal(Return (Class::*)(Arg...), Class *obj, Arg &&...args)
    {
        auto func = reinterpret_cast<Return (*)(Class *, Arg...)>(getOriginalVirtualMethod<index>());
        return func(obj, std::forward<Arg>(args)...);
    }

    template <std::size_t index, typename Return, typename Class, typename... Arg>
    Return *callOriginalRvo(Return (Class::*)(Arg...), Return *ret, Class *obj, Arg &&...args)
    {
#ifdef _WIN32
        auto func = reinterpret_cast<Return *(*)(Class *, Return *, Arg...)>(getOriginalVirtualMethod<index>());
        return func(obj, ret, std::forward<Arg>(args)...);
#elif __linux__
        auto func = reinterpret_cast<Return *(*)(Return *, Class *, Arg...)>(getOriginalVirtualMethod<index>());
        return func(ret, obj, std::forward<Arg>(args)...);
#endif
    }

protected:
    template <std::size_t index>
    void hook(void *detour)
    {
        if (index >= size_) {
            spdlog::critical("VMT hook failed. Invalid index: {}. Size: {}.", index, size_);
            std::terminate();
        }
        // TODO: check if already hooked
        copy_[index] = reinterpret_cast<std::uintptr_t>(detour);
    }

    template <std::size_t index>
    std::uintptr_t getOriginalVirtualMethod()
    {
        if (index >= size_) {
            spdlog::critical("Invalid index: {}. Size: {}.", index, size_);
            std::terminate();
        }
        return original_[index];
    }

private:
    constexpr static std::size_t type_info_size = _WIN32_LINUX_(1, 2);
    std::uintptr_t *original_;
    std::unique_ptr<std::uintptr_t[]> copy_;
    std::size_t size_;
};

}  // namespace endstone::detail
