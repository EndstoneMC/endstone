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
#include "bedrock/common_types.h"
#include "bedrock/core/utility/unique_owner_pointer.h"
#include "bedrock/forward.h"
#include "bedrock/minecraft_app_interface.h"
#include "bedrock/server/console_input_reader.h"
#include "bedrock/world/events/server_instance_event_coordinator.h"

class DedicatedServer : public IMinecraftApp, public Bedrock::AppIsland {
public:
    enum class StartResult : int {
        Success = 0,
        PortOccupied = 1,
        InvalidSettings = 2,
        MissingDependency = 3,
        RuntimeError = 4,
    };
    DedicatedServer();
    ~DedicatedServer() override;
    ENDSTONE_HOOK StartResult start(const std::string &sessionID, const Bedrock::ActivationArguments &args);
    bool stop() override;
    [[nodiscard]] virtual Bedrock::NotNullNonOwnerPtr<FileArchiver> getFileArchiver() const;

private:
    virtual Bedrock::NotNullNonOwnerPtr<Minecraft> getPrimaryMinecraft();
    [[nodiscard]] Bedrock::NotNullNonOwnerPtr<Automation::AutomationClient> getAutomationClient() const override;
    [[nodiscard]] bool isEduMode() const override;
    [[nodiscard]] bool isDedicatedServer() const override;
    void onNetworkMaxPlayersChanged(uint32_t newMaxPlayerCount) override;
    virtual IGameModuleShared &getGameModuleShared();
    void teardownHttp();
    void requestServerShutdown(const std::string &) override;
    virtual bool requestInGamePause(const SubClientId &, bool);

    Bedrock::NonOwnerPointer<Minecraft> minecraft_;
    Bedrock::NonOwnerPointer<Core::FileSystem> file_system_;
    Bedrock::UniqueOwnerPointer<ServerInstanceEventCoordinator> server_instance_event_coordinator_;
    std::atomic<bool> wants_to_quit_;
    std::atomic<StartResult> result_;
    std::unique_ptr<ConsoleInputReader> console_input_reader_;
    //...
};
