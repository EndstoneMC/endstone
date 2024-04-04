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

#include <cstddef>
#include <functional>
#include <unordered_map>

#include <spdlog/spdlog.h>

#include "endstone/detail/cast.h"

namespace endstone::detail {

template <typename T>
class VirtualTable {
public:
    explicit VirtualTable(T &target) : target_(target)
    {
        original_ = *(reinterpret_cast<uintptr_t **>(&target_));
        size_ = 0;
        while (original_[size_]) {
            ++size_;
        }

        copy_ = new uintptr_t[size_ + 1];
        std::copy(original_, original_ + size_, copy_);
        copy_[size_] = 0;
        *reinterpret_cast<uintptr_t **>(&target_) = copy_;
    };

    VirtualTable(const VirtualTable &) = delete;
    VirtualTable &operator=(const VirtualTable &) = delete;
    VirtualTable(VirtualTable &&) = delete;
    VirtualTable &operator=(VirtualTable &&) = delete;

    ~VirtualTable()
    {
        *reinterpret_cast<uintptr_t **>(&target_) = original_;
        delete[] copy_;
    }

    template <size_t index, typename Return, typename Class, typename... Arg>
    void hook(Return (Class::*fp)(Arg...))
    {
        hook<index>(fp_cast(fp));
    }

    template <size_t index, typename Return, typename Class, typename... Arg>
    void hook(Return (Class::*fp)(Arg...) const)
    {
        hook<index>(fp_cast(fp));
    }

    template <size_t index>
    void unhook()
    {
        if (index >= size_) {
            spdlog::critical("VMT unhook failed. Invalid index: {}. Size: {}", index, size_);
        }
        copy_[index] = original_[index];
    }

    template <typename Return, typename Class, typename... Arg>
    Return callOriginal(Return (Class::*fp)(Arg...), Arg &&...args)
    {
        auto func = reinterpret_cast<Return (*)(T *, Arg...)>(getOriginal(fp_cast(fp)));
        return func(&target_, std::forward<Arg>(args)...);
    }

    template <typename Return, typename Class, typename... Arg>
    Return callOriginal(Return (Class::*fp)(Arg...) const, Arg &&...args)
    {
        auto func = reinterpret_cast<Return (*)(const T *, Arg...)>(getOriginal(fp_cast(fp)));
        return func(&target_, std::forward<Arg>(args)...);
    }

private:
    template <size_t index>
    void hook(void *detour)
    {
        if (index >= size_) {
            spdlog::critical("VMT hook failed. Invalid index: {}. Size: {}", index, size_);
            std::terminate();
        }
        index_lookup_[detour] = index;
        copy_[index] = reinterpret_cast<uintptr_t>(detour);
    }

    void *getOriginal(void *detour) const
    {
        auto it = index_lookup_.find(detour);
        if (it == index_lookup_.end()) {
            spdlog::critical("No original function can be found for {}!!", detour);
            std::terminate();
        }
        return reinterpret_cast<void *>(original_[it->second]);  // NOLINT(*-no-int-to-ptr)
    }

    T &target_;
    uintptr_t *copy_;
    uintptr_t *original_;
    std::unordered_map<void *, size_t> index_lookup_;
    size_t size_;
};

}  // namespace endstone::detail
