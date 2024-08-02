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

#include "bedrock/world/scores/server_scoreboard.h"

#include <bedrock/world/level/level.h>

#include "endstone/detail/hook.h"

std::unique_ptr<ServerScoreboard> ServerScoreboard::create(Level &level)
{
    std::string name = __FUNCDNAME__;
    CommandSoftEnumRegistry registry;
    auto *scoreboard = endstone::detail::hook::call_ctor<ServerScoreboard>(
#ifdef _WIN32
        "??0ServerScoreboard@@QEAA@VCommandSoftEnumRegistry@@PEAVLevelStorage@@AEBV?$not_null@V?$StackRefResult@"
        "VGameplayUserManager@@@@@gsl@@@Z",
#else
        "_ZN16ServerScoreboardC2E23CommandSoftEnumRegistryP12LevelStorageRKN3gsl8not_"
        "nullI14StackRefResultI19GameplayUserManagerEEE",
#endif
        registry, nullptr, level._getGameplayUserManagerStackRef());
    return std::unique_ptr<ServerScoreboard>(scoreboard);
}
