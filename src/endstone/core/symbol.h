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

#include <stdexcept>
#include <string_view>

#include "cast.h"

namespace endstone::core {

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

#define ENDSTONE_SYMCALL(fp, ...) \
    std::invoke(endstone::core::fp_cast(fp, endstone::core::get_symbol_addr(__FUNCDNAME__)), ##__VA_ARGS__)

}  // namespace endstone::core
