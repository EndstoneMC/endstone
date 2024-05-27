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

#include "bedrock/bedrock.h"
#include "bedrock/core/app_platform_listener.h"
#include "bedrock/core/memory.h"
#include "bedrock/forward.h"
#include "bedrock/world/game_callbacks.h"
#include "bedrock/world/minecraft.h"

class ServerInstance : public Bedrock::EnableNonOwnerReferences,
                       public AppPlatformListener,
                       public GameCallbacks,
                       public Core::StorageAreaStateListener {
public:
    Minecraft &getMinecraft();

private:
    std::chrono::steady_clock::time_point last_sync_time_;  // +152
    IMinecraftApp *app_;                                    // +160
    std::unique_ptr<Minecraft> minecraft_;                  // +168
};
