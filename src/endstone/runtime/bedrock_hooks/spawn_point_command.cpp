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

#include "bedrock/server/commands/standard/spawn_point_command.h"

#include "bedrock/server/commands/command_origin.h"
#include "bedrock/world/level/level.h"
#include "endstone/core/entity/components/flag_components.h"
#include "endstone/runtime/hook.h"

void SpawnPointCommand::execute(const CommandOrigin &origin, CommandOutput &output) const
{
    auto level_entity = origin.getLevel()->getLevelEntity().unwrap();
    if (level_entity) {
        level_entity->getOrAddComponent<endstone::core::InternalSpawnPointCommandFlagComponent>();
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&SpawnPointCommand::execute, this, origin, output);
    if (level_entity) {
        level_entity->removeComponent<endstone::core::InternalSpawnPointCommandFlagComponent>();
    }
}
