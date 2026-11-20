// Copyright (c) 2026, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <algorithm>
#include <cstddef>
#include <cstdint>

#include "endstone/runtime/hook.h"

#if defined(__linux__) || defined(_WIN32)

#ifdef _WIN32
#define ONHIT_EXPORT __declspec(dllexport)
#else
#define ONHIT_EXPORT __attribute__((visibility("default")))
#endif

namespace {
struct OnHitChildren {
    void **begin;
    void **end;
    void **capacity;
};

static_assert(sizeof(OnHitChildren) == 3 * sizeof(void *));

void reclaimChildren(void *record)
{
    auto &children = *reinterpret_cast<OnHitChildren *>(static_cast<std::byte *>(record) + 0x120);
    if (!children.begin) {
        return;
    }

    for (auto *it = children.begin; it != children.end; ++it) {
        auto *child = *it;
        if (!child || std::find(children.begin, it, child) != it) {
            continue;
        }
        auto **vtable = *reinterpret_cast<void ***>(child);
#ifdef _WIN32
        // MSVC scalar deleting destructor: RCX=this, RDX=1 deallocates the child.
        reinterpret_cast<void (*)(void *, unsigned int)>(vtable[0])(child, 1);
#else
        reinterpret_cast<void (*)(void *)>(vtable[1])(child);
#endif
    }
}
}  // namespace

// BDS otherwise discards the previous OnHit children when it resets end = begin.
extern "C" ONHIT_EXPORT std::uintptr_t endstone_onhit_initialize(void *record, void *actor)
{
    reclaimChildren(record);
    return ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&endstone_onhit_initialize, "endstone_onhit_initialize", record, actor);
}

// BDS destroys the pointer array but not the owned subcomponents.
extern "C" ONHIT_EXPORT std::uintptr_t endstone_onhit_cleanup(void *record)
{
    reclaimChildren(record);
    return ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&endstone_onhit_cleanup, "endstone_onhit_cleanup", record);
}

#undef ONHIT_EXPORT
#endif
