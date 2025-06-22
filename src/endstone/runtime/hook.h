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

#include <system_error>

#include <entt/entt.hpp>

#include "bedrock/symbol.h"
#include "endstone/detail/cast.h"

namespace endstone::hook {
namespace details {
const std::error_category &error_category();
void *&get_original(void *target);
const std::unordered_map<std::string, void *> &get_targets();
const std::unordered_map<std::string, void *> &get_detours();
}  // namespace details

void install();
template <std::size_t RVA>
void *get_original()
{
    static void **original = nullptr;
    if (!original) {
        original = &details::get_original(static_cast<char *>(detail::get_executable_base()) + RVA);
    }
    return *original;
}
}  // namespace endstone::hook

#define ENDSTONE_HOOK_CALL_ORIGINAL(fp, ...) ENDSTONE_HOOK_CALL_ORIGINAL_NAME(fp, __FUNCDNAME__, ##__VA_ARGS__)
#define ENDSTONE_HOOK_CALL_ORIGINAL_NAME(fp, name, ...)                                                            \
    std::invoke(endstone::detail::fp_cast(fp, endstone::hook::get_original<endstone::detail::get_symbol(name)>()), \
                ##__VA_ARGS__)
