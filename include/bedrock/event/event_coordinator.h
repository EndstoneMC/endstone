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
#include "bedrock/event/actor_event.h"
#include "bedrock/event/block_event.h"
#include "bedrock/event/coordinator_result.h"
#include "bedrock/event/event_ref.h"
#include "bedrock/event/item_event.h"
#include "bedrock/event/level_event.h"
#include "bedrock/event/player_event.h"
#include "bedrock/event/server_event.h"
#include "bedrock/forward.h"
#include "bedrock/gameplay_handler.h"
#include "bedrock/server/server_instance.h"

template <typename Listener>
class EventCoordinatorPimpl : public Bedrock::EnableNonOwnerReferences {
private:
    std::vector<Listener *> unknown1_;                              // +24
    std::vector<std::function<EventResult(Listener &)>> unknown2_;  // +48
    std::vector<Listener *> unknown3_;                              // +72
    bool unknown4_;                                                 // +96
    int unknown5_;                                                  // +100
};
static_assert(sizeof(EventCoordinatorPimpl<class T>) == 104);

class ActorEventCoordinator;
class BlockEventCoordinator;
class ItemEventCoordinator;

class LevelEventCoordinator : public EventCoordinatorPimpl<class LevelEventListener> {
public:
    ~LevelEventCoordinator() override = default;

private:
#ifdef __linux__
    int64_t unknown6_;
#endif
    bool unknown7_;  // +104
    int unknown8_;   // +108

public:
    std::unique_ptr<LevelGameplayHandler> gameplay_handler;  // +112
};

class PlayerEventCoordinator;
class ServerPlayerEventCoordinator;
class ClientPlayerEventCoordinator;

class ServerInstanceEventCoordinator {
public:
    BEDROCK_API void sendServerInitializeStart(ServerInstance &instance);
    BEDROCK_API void sendServerThreadStarted(ServerInstance &instance);
    BEDROCK_API void sendServerThreadStopped(ServerInstance &instance);
    BEDROCK_API void sendServerLevelInitialized(ServerInstance &instance, Level &level);
};

class ServerNetworkEventCoordinator;
class ScriptingEventCoordinator;
class ScriptDeferredEventCoordinator;
