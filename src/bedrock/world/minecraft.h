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

#include "bedrock/entity/gamerefs_entity/entity_registry_owner.h"
#include "bedrock/forward.h"
#include "bedrock/minecraft_app_interface.h"
#include "bedrock/network/server_network_handler.h"
#include "bedrock/resources/resource_pack_manager.h"
#include "bedrock/server/commands/minecraft_commands.h"
#include "bedrock/util/timer.h"
#include "bedrock/world/game_callbacks.h"
#include "bedrock/world/game_session.h"

struct MinecraftArguments {
    using RequestServerShutdownCallback = brstd::move_only_function<void() const>;
};

class Minecraft : public IEntityRegistryOwner {
public:
    Minecraft(MinecraftArguments &&args);
    ~Minecraft() override = default;
    MinecraftCommands &getCommands();
    [[nodiscard]] Bedrock::NonOwnerPointer<ServerNetworkHandler> getServerNetworkHandler() const;
    void requestServerShutdown();
    void requestResourceReload();
    [[nodiscard]] ResourcePackManager &getResourceLoader() const;
    [[nodiscard]] Level *getLevel() const;

private:
    GameCallbacks &game_callbacks_;
    IMinecraftEventing &eventing_;
    std::unique_ptr<ResourcePackManager> resource_loader_;
    std::unique_ptr<StructureManager> structure_manager_;
    std::shared_ptr<GameModuleServer> game_module_server_;
    // AllowList &allow_list_;
    PermissionsFile *permissions_file_;
    std::unique_ptr<PrivateKeyManager> server_keys_;
    std::unique_ptr<MinecraftServiceKeyManager> minecraft_service_keys_;
    std::string save_game_path_;
    // Bedrock::NonOwnerPointer<Core::FilePathManager> file_path_manager_;
    ServerMetrics *server_metrics_;
    const bool is_dedicated_server_;
    bool corruption_detected_;
    bool fire_on_level_corrupt_;
    double frame_duration_;
    double last_frame_start_;
    std::chrono::seconds max_player_idle_time_;
    std::unique_ptr<DefaultCommandsContextProvider> default_commands_context_provider_;
    std::unique_ptr<MinecraftCommands> commands_;
    std::unique_ptr<GameSession> game_session_;
    void *game_test_level_listener_;
    void *game_test_;
    void *sim_timer_;
    void *real_timer_;
    ClientOrServerNetworkSystemRef network_;
    PacketSender *packet_sender_;
    SubClientId client_sub_id_;
    OwnerPtr<EntityRegistry> entity_registry_;
    std::unique_ptr<ITickingSystem> add_movement_tick_for_catchup_;
    Bedrock::PubSub::PublisherPtr<void(Level *), Bedrock::PubSub::ThreadModel::SingleThreaded> level_subscribers_;
    MinecraftArguments::RequestServerShutdownCallback request_server_shutdown_;
    //...
};
