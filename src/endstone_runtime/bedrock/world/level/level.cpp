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

#include "bedrock/world/level/level.h"

#include <cpptrace/cpptrace.hpp>

#include "endstone/detail/hook.h"
#include "endstone/detail/scheduler/scheduler.h"
#include "endstone/detail/server.h"

void Level::tick()
{
    using endstone::detail::EndstoneScheduler;
    using endstone::detail::EndstoneServer;
    auto &server = entt::locator<EndstoneServer>::value();
    auto &scheduler = static_cast<EndstoneScheduler &>(server.getScheduler());
    scheduler.mainThreadHeartbeat(getCurrentServerTick().tick_id);

    ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&Level::tick, __FUNCDNAME__, this);
}
