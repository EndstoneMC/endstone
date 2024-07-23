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

#pragma once

#include <string>

#include "bedrock/bedrock.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/actor/player/build_platform.h"
#include "bedrock/world/actor/player/player.h"

class PlayerListEntry {
public:
    ActorUniqueID unique_id;                                       // +0
    mce::UUID uuid;                                                // +8
    std::string name;                                              // +24
    std::string xuid;                                              // +56
    std::string platform_id;                                       // +88
    BuildPlatform platform;                                        // +120
    size_t serialized_skin[ENDSTONE_VARIANT_WIN32_LINUX(76, 63)];  // +128 // TODO(fixme): figure out the structure
    bool is_teacher;                                               // +736
    bool is_hosting_player;                                        // +737
    bool is_sub_client;                                            // +738
};
