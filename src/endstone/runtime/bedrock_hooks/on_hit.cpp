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

#include <algorithm>
#include <cstddef>
#include <cstdint>

#include "bedrock/entity/components/reconstructed_on_hit_record.h"
#include "endstone/runtime/hook.h"

namespace {
struct OnHitChildren {
    void **begin;
    void **end;
    void **capacity;
};

static_assert(sizeof(OnHitChildren) == 3 * sizeof(void *));

void reclaimChildren(void *record)
{
    // OnHit child-pointer vector starts at record +0x120.
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
std::uintptr_t ReconstructedOnHitRecord::initialize(Actor *actor)
{
    reclaimChildren(this);
    return ENDSTONE_HOOK_CALL_ORIGINAL(&ReconstructedOnHitRecord::initialize, this, actor);
}

// BDS destroys the pointer array but not the owned subcomponents.
std::uintptr_t ReconstructedOnHitRecord::cleanup()
{
    reclaimChildren(this);
    return ENDSTONE_HOOK_CALL_ORIGINAL(&ReconstructedOnHitRecord::cleanup, this);
}
