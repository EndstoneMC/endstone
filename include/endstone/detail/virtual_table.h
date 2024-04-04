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
    bool hook(Return (Class::*fp)(Arg...))
    {
        return hook<index>(fp_cast(fp));
    }

    template <size_t index, typename Return, typename Class, typename... Arg>
    bool hook(Return (Class::*fp)(Arg...) const)
    {
        return hook<index>(fp_cast(fp));
    }

    template <size_t index>
    bool unhook()
    {
        if (index >= size_) {
            return false;
        }
        copy_[index] = original_[index];
        return true;
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
    bool hook(void *detour)
    {
        if (index >= size_) {
            return false;
        }
        copy_[index] = reinterpret_cast<uintptr_t>(detour);
        return true;
    }

    void *getOriginal(void *detour) const
    {
        auto it = hooks_.find(detour);
        if (it == hooks_.end()) {
            return nullptr;
        }
        return it->second;
    }

    T &target_;
    uintptr_t *copy_;
    uintptr_t *original_;
    std::unordered_map<void *, void *> hooks_;
    size_t size_;
};

}  // namespace endstone::detail
