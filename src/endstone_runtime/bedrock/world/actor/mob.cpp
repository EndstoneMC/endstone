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

#include "bedrock/world/actor/mob.h"

#include "bedrock/world/actor/actor_flags.h"
#include "endstone/detail/hook.h"

void Mob::die(const ActorDamageSource &source)
{
#if _WIN32
    ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&Mob::die, __FUNCDNAME__, this, source);
#else
    ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&Mob::die, "_ZN3Mob3dieERK17ActorDamageSource", this, source);
#endif
}

bool Mob::isGliding() const
{
    return getStatusFlag(ActorFlags::GLIDING);
}
