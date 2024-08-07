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
#include <memory>
#include <string>

#include "bedrock/core/file.h"
#include "bedrock/core/memory.h"
#include "bedrock/forward.h"
#include "bedrock/network/server_network_handler.h"
#include "bedrock/server/commands/minecraft_commands.h"
#include "bedrock/world/game_callbacks.h"
#include "bedrock/world/game_session.h"
#include "bedrock/world/minecraft_app.h"

class Minecraft : public Bedrock::EnableNonOwnerReferences {
public:
    ~Minecraft() override = 0;
    MinecraftCommands &getCommands();
    [[nodiscard]] Bedrock::NonOwnerPointer<ServerNetworkHandler> getServerNetworkHandler() const;
    void requestServerShutdown(const std::string &message);
    void requestResourceReload();

private:
    GameCallbacks *game_callbacks_;                                           // +24
    IMinecraftEventing *eventing_;                                       // +32
    std::unique_ptr<ResourcePackManager> resource_loader_;               // +40
    std::unique_ptr<StructureManager> structure_manager_;                // +48
    std::shared_ptr<GameModuleServer> game_module_server_;               // +56
    AllowList *allow_list_;                                              // +72
    PermissionsFile *permissions_file_;                                  // +80
    std::unique_ptr<PrivateKeyManager> server_keys_;                     // +88
    std::string save_game_path_;                                         // +96
    Bedrock::NonOwnerPointer<Core::FilePathManager> file_path_manager_;  // +128
    ServerMetrics *server_metrics_;                                      // +144
    bool corruption_detected_;                                           // +152
    double frame_duration_;                                              // +160
    double last_update_;                                                 // +168
    std::chrono::seconds max_player_idle_time_;                          // +176
    std::unique_ptr<MinecraftCommands> commands_;                        // +184
    std::unique_ptr<GameSession> game_session_;                          // +192
    void *game_test_level_listener_;                                     // +200
    void *game_test_;                                                    // +208
    void *sim_timer_;                                                    // +216
    void *real_timer_;                                                   // +224
    std::variant<void *> network_;                                       // +232
    PacketSender *packet_sender_;                                        // +248
    IMinecraftApp *app_;                                                 // +256
};
