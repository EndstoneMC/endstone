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

#include "bedrock/world/actor/player/player_item_in_use.h"

#include "bedrock/entity/components/item_in_use_component.h"

void PlayerItemInUse::clearItemInUse(EntityContext &entity)
{
    item_.setNull(std::nullopt);
    entity.removeComponent<ItemInUseComponent>();
    entity.removeComponent<ItemInUseTicksDuringMovementComponent>();
    slot_ = {-1, CONTAINER_ID_NONE};
}
