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

#include <chrono>

#include "bedrock/command/minecraft_commands.h"
#include "bedrock/core.h"
#include "bedrock/forward.h"
#include "bedrock/memory.h"

class Minecraft : public Bedrock::EnableNonOwnerReferences {
public:
    virtual ~Minecraft() = 0;
    MinecraftCommands &getCommands();

private:
    BEDROCK_API void tickSimtime(int, int);

    GameCallbacks &callbacks_;                                           // +24
    IMinecraftEventing &eventing_;                                       // +32
    std::unique_ptr<ResourcePackManager> resource_pack_manager_;         // +40
    std::unique_ptr<StructureManager> structure_manager_;                // +48
    std::shared_ptr<GameModuleServer> game_module_server_;               // +56
    AllowList &allow_list_;                                              // +72
    PermissionsFile *permissions_file_;                                  // +80
    std::unique_ptr<PrivateKeyManager> private_key_manager_;             // +88
    std::string unk3_;                                                   // +96
    Bedrock::NonOwnerPointer<Core::FilePathManager> file_path_manager_;  // +128
    ServerMetrics &metrics_;                                             // +144
    bool unk4_;                                                          // +152
    bool unk5_;                                                          // +153
    double elapsed_time_seconds_;                                        // +160
    double last_update_seconds_;                                         // +168
    std::chrono::seconds unk6_;                                          // +176
    std::unique_ptr<MinecraftCommands> commands_;                        // +184
};
