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

#include "bedrock/entity/components/container_component.h"

#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/level/level.h"
#include "endstone/core/entity/components/flag_components.h"
#include "endstone/core/inventory/inventory.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/inventory/inventory_open_event.h"
#include "endstone/runtime/hook.h"

bool ContainerComponent::openContainer(Actor &container_actor, Player &player)
{
    if (!canOpenContainer(container_actor, player)) {
        return false;
    }
    unpackLootTable(player.getLevel(), player.getDimensionId(), container_actor);

    endstone::core::EndstoneInventory inventory{[this]() -> ::Container & { return container_; }};
    endstone::InventoryOpenEvent e{inventory, player.getEndstoneActor<endstone::core::EndstonePlayer>()};
    endstone::core::EndstoneServer::getInstance().getPluginManager().callEvent(e);
    if (e.isCancelled()) {
        containerClosed(player);
        return false;
    }

    player.addOrRemoveComponent<endstone::core::InternalEntityContainerOpenFlagComponent>(true);
    const auto opened = ENDSTONE_HOOK_CALL_ORIGINAL(&ContainerComponent::openContainer, this, container_actor, player);
    player.addOrRemoveComponent<endstone::core::InternalEntityContainerOpenFlagComponent>(false);
    return opened;
}
