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

#include "bedrock/app_platform_listener.h"
#include "bedrock/core/file/storage_area_state_listener.h"
#include "bedrock/core/threading/spsc_queue.h"
#include "bedrock/core/utility/service_locator.h"
#include "bedrock/network/loopback_packet_sender.h"
#include "bedrock/platform/threading/condition_variable.h"
#include "bedrock/platform/threading/thread.h"
#include "bedrock/scripting/server_script_manager.h"
#include "bedrock/server/cdn_config.h"
#include "bedrock/server/server_text_settings.h"
#include "bedrock/util/timer.h"
#include "bedrock/world/events/server_instance_event_coordinator.h"
#include "bedrock/world/game_callbacks.h"
#include "bedrock/world/game_session.h"
#include "bedrock/world/minecraft.h"

class ServerInstance : public Bedrock::EnableNonOwnerReferences,
                       public AppPlatformListener,
                       public GameCallbacks,
                       public Core::StorageAreaStateListener {
public:
    ServerInstance(IMinecraftApp &, const Bedrock::NotNullNonOwnerPtr<ServerInstanceEventCoordinator> &);
    enum InstanceState : unsigned int {
        Running = 0,
        Suspended = 1,
        WaitingLeaveGame = 2,
        Stopped = 3,
        NotStarted = 4,
    };

    Minecraft *getMinecraft();
    ServerNetworkSystem &getNetwork();
    PacketSender &getPacketSender();
    Bedrock::NotNullNonOwnerPtr<ServerInstanceEventCoordinator> getEventCoordinator();
    Bedrock::NonOwnerPointer<CDNConfig> getCDNConfig() const;
    Bedrock::NonOwnerPointer<ServerTextSettings> getServerTextSettings() const;

public:
    std::chrono::steady_clock::time_point last_sync_time;

private:
    friend class endstone::core::EndstoneServer;
    const bool is_dedicated_server_;
    std::unique_ptr<Minecraft> minecraft_;
    std::unique_ptr<ProfilingManager> profiling_manager;
    ServiceRegistrationToken<ProfilingManager> profiling_manager_service_registration_token_;
    std::unique_ptr<ServerNetworkSystem> network_;
    std::unique_ptr<LoopbackPacketSender> packet_sender_;
    std::unique_ptr<Timer> sim_timer_;
    std::unique_ptr<Timer> real_timer_;
    std::unique_ptr<Scheduler> scheduler_;
    std::unique_ptr<EducationOptions> education_options_;
    LevelStorage *storage_;
    std::atomic_bool in_update_;
    std::atomic<int> write_ref_counter_;
    std::atomic_bool thread_should_join_;
    Bedrock::Threading::Mutex mutex_destruction_;
    Bedrock::NotNullNonOwnerPtr<ServerInstanceEventCoordinator> event_coordinator_;
    std::atomic<InstanceState> instance_state_;
    Bedrock::Threading::Thread server_instance_thread_;
    Bedrock::Threading::Mutex resume_mutex_;
    Bedrock::Threading::ConditionVariable resume_signal_;
    std::unique_ptr<ServerScriptManager> server_script_manager_;
    std::unique_ptr<Scripting::RegistryManager> script_registry_manager_;
    std::function<void(const char *)> script_watchdog_critical_error_callback_;
    std::function<void(Connection::DisconnectFailReason, const char *)> unrecoverable_error_callback_;
    bool handled_level_corruption_;
    std::unique_ptr<TextFilteringProcessor> text_filtering_processor_;
    std::chrono::microseconds wakeup_interval_;
    std::string level_id_;
    std::string server_name_;
    std::unique_ptr<WorldSessionEndPoint> world_session_end_point_;
    std::shared_ptr<Core::FileStorageArea> storage_area_for_level_;
    std::shared_ptr<ItemRegistry> server_item_registry_;
    bool enable_item_stack_net_manager_;
    bool enable_realms_stories_;
    bool initialized_;
    bool flagged_for_early_destruction_;
    ServiceRegistrationToken<ServerInstance> service_registration_token_;
    bool has_scheduled_leave_game_;
    bool critical_disk_error_telemetry_sent_;
    std::unique_ptr<Editor::IEditorManager> editor_manager_;
    Bedrock::NonOwnerPointer<CDNConfig> cdn_config_;
    Bedrock::NonOwnerPointer<ServerTextSettings> server_text_settings_;  // +952
    Bedrock::NotNullNonOwnerPtr<cereal::ReflectionCtx> cereal_context_;
    std::unique_ptr<LinkedAssetValidator> linked_asset_validator_;
};
