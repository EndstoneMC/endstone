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

#include "bedrock/util/pushable_by_entity_utility.h"

#include "bedrock/world/actor/actor.h"
#include "endstone/core/server.h"
#include "endstone/event/actor/actor_collide_with_actor_event.h"
#include "endstone/runtime/hook.h"

bool PushableByEntityUtility::skipPush(Actor &owner, Actor &other)
{
    if (PushableByEntityUtility::SkipPushBypassScope::consume(owner, other)) {
        return false;
    }

    constexpr auto symbol = __FUNCDNAME__;
    return ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&PushableByEntityUtility::skipPush, symbol, owner, other);
}

void PushableByEntityUtility::push(Actor &owner, Actor &other, bool push_self_only)
{
    // skipPush is the native precondition used by push.
    if (PushableByEntityUtility::skipPush(owner, other)) {
        return;
    }

    const auto &server = endstone::core::EndstoneServer::getInstance();
    endstone::ActorCollideWithActorEvent event{owner.getEndstoneActor<endstone::Actor>(),
                                               other.getEndstoneActor<endstone::Actor>()};
    server.getPluginManager().callEvent(event);

    if (event.isCancelled()) {
        return;
    }

    // BDS checks skipPush again at push's entry; reuse the pre-event result for that check.
    PushableByEntityUtility::SkipPushBypassScope skip_push_bypass{owner, other};
    ENDSTONE_HOOK_CALL_ORIGINAL(&PushableByEntityUtility::push, owner, other, push_self_only);
}
