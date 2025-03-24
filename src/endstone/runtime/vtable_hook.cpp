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

#include "endstone/runtime/vtable_hook.h"

#include <funchook.h>

#include <unordered_map>

namespace endstone::hook {
namespace details {
using OriginalMap = std::unordered_map<void *, void *>;
static OriginalMap &originals()  // NOLINT(*-use-anonymous-namespace)
{
    static OriginalMap originals;
    return originals;
}

void install_vtable(void **vtable, int ordinal, void *detour)
{
    // Get the function pointer at the specified ordinal.
    void *target = vtable[ordinal];

    // Create a funchook instance.
    funchook_t *funchook = funchook_create();
    if (!funchook) {
        fprintf(stderr, "Failed to create funchook instance\n");
        exit(EXIT_FAILURE);
    }

    // Prepare the hook: provide the address of the target pointer and the detour.
    int status = funchook_prepare(funchook, &target, detour);
    if (status != 0) {
        fprintf(stderr, "Error in funchook_prepare: %s\n", funchook_error_message(funchook));
        exit(EXIT_FAILURE);
    }

    // Install the hook.
    status = funchook_install(funchook, 0);
    if (status != 0) {
        fprintf(stderr, "Error in funchook_install: %s\n", funchook_error_message(funchook));
        exit(EXIT_FAILURE);
    }
    originals()[detour] = target;
}
}  // namespace details

void *get_vtable_original(void *detour)
{
    const auto it = details::originals().find(detour);
    if (it == details::originals().end()) {
        throw std::runtime_error("original function not found");
    }
    return it->second;
}
}  // namespace endstone::hook
