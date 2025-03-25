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

#include <mutex>

#include "endstone/detail/cast.h"

namespace endstone::hook {
namespace details {
void hook_vtable(void **vtable, int ordinal, void *detour);
}

template <int Ordinal, typename Obj, typename Detour>
void hook_vtable(Obj *obj, Detour detour)
{
    void **vtable = *reinterpret_cast<void ***>(obj);
    details::hook_vtable(vtable, Ordinal, detail::fp_cast(detour));
}

void *get_vtable_original(void *detour);
}  // namespace endstone::hook

#define ENDSTONE_VHOOK_CALL_ORIGINAL(fp, ...)                                                               \
    std::invoke(endstone::detail::fp_cast(fp, endstone::hook::get_vtable_original(endstone::detail::fp_cast(fp))), \
                ##__VA_ARGS__)
