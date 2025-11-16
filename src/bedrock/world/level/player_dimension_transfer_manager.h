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

#include "bedrock/forward.h"
#include "bedrock/world/level/player_dimension_transferer.h"

class PlayerDimensionTransferManager {
public:
    PlayerDimensionTransferManager(std::unique_ptr<PlayerLimboActorManager>,
                                   std::unique_ptr<IPlayerDimensionTransferer>,
                                   Bedrock::NotNullNonOwnerPtr<DimensionManager>,
                                   Bedrock::NotNullNonOwnerPtr<GameplayUserManager>,
                                   const gsl::not_null<StackRefResult<EntityRegistry>> &,
                                   std::unique_ptr<ILevelCrashDumpManager>);

    IPlayerDimensionTransferConnector &getPlayerDimensionTransferConnector()
    {
        return *player_dimension_transferer_;
    }

private:
    const std::unique_ptr<PlayerLimboActorManager> player_limbo_actor_manager_;
    const gsl::not_null<std::unique_ptr<IPlayerDimensionTransferer>> player_dimension_transferer_;
    const Bedrock::NotNullNonOwnerPtr<GameplayUserManager> gameplay_user_manager_;
    const Bedrock::NotNullNonOwnerPtr<DimensionManager> dimension_manager_;
    const WeakRef<EntityRegistry> entity_registry_;
    const gsl::not_null<std::unique_ptr<ILevelCrashDumpManager>> level_crash_dump_manager_;
    bool level_finished_initializing_;
    Bedrock::PubSub::Subscription on_gameplay_user_removed_;
};
