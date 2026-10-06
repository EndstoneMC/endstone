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

#include "bedrock/world/redstone/circuit/circuit_scene_graph.h"

#include <ranges>
#include <vector>

#include "endstone/runtime/hook.h"

void CircuitSceneGraph::update(BlockSource *region)
{
    // #blameMojang - a redstone torch keeps a list of the blocks it powers. BDS rebuilds that list when the
    // circuit changes but keeps every block that is no longer there. Break the block above a torch and put
    // it back, and the torch lists it twice. A piston clock does this all day.
    // Fix: drop the missing blocks before BDS rebuilds the list.
    for (const auto &entry : pending_updates_ | std::views::values) {
        const auto it = power_association_map_.find(entry.pos);
        if (it == power_association_map_.end()) {
            continue;
        }
        std::erase_if(it->second.components, [this](const CircuitComponentList::Item &item) {
            return !all_components_.contains(item.pos) && !pending_adds_.contains(item.pos);
        });
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&CircuitSceneGraph::update, this, region);
}
