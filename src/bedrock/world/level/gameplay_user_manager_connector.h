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

#include "bedrock/core/utility/pub_sub/connector.h"
#include "bedrock/entity/gamerefs_entity/entity_context.h"
#include "bedrock/world/actor/player/player.h"

class IGameplayUserManagerConnector {
public:
    virtual Bedrock::PubSub::Connector<void(EntityContext &)> &getGameplayUserAddedConnector() = 0;
    virtual Bedrock::PubSub::Connector<void(const EntityContext &)> &getGameplayUserResumedConnector() = 0;
    virtual Bedrock::PubSub::Connector<void(const EntityContext &)> &getGameplayUserSuspendedConnector() = 0;
    virtual Bedrock::PubSub::Connector<void(const Player &)> &getPlayerRenamedConnector() = 0;
    virtual Bedrock::PubSub::Connector<void(EntityContext &)> &getGameplayUserRemovedConnector() = 0;
    virtual Bedrock::PubSub::Connector<void()> &getAnyGameplayUsersRemovedConnector() = 0;
};
