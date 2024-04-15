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

#include "bedrock/actor/actor_manager.h"

#include "bedrock/actor/components/actor_identifier.h"
#include "bedrock/actor/entity_context.h"
#include "endstone/detail/hook.h"

Actor *ActorManager::addActorEntity(IAddActorEntityProxy &proxy, OwnerPtr<EntityContext> ctx)
{
    auto *actor = ENDSTONE_HOOK_CALL_ORIGINAL(&ActorManager::addActorEntity, this, proxy, ctx);
    // TODO(event): call ActorSpawnEvent (except for Player)
    //  if cancelled, return nullptr to prevent the spawn,
    //  otherwise call the original function and return the result
    // TODO(actor): create endstone::Actor and add to endstone::Level
    return actor;
}
