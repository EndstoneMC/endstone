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

#include <thread>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/forward.h"
#include "bedrock/world/events/actor_event.h"
#include "bedrock/world/events/event_ref.h"
#include "bedrock/world/events/gameplay_handler.h"
#include "bedrock/world/events/item_event.h"
#include "bedrock/world/events/level_event.h"
#include "bedrock/world/events/player_event.h"
#include "bedrock/world/events/server_event.h"

class ServerInstance;
class Level;

template <typename ListenerType>
class EventCoordinatorPimpl : public Bedrock::EnableNonOwnerReferences {
public:
    ~EventCoordinatorPimpl() override = 0;

private:
    std::vector<ListenerType *> listeners_;                                      // +24
    std::vector<std::function<EventResult(ListenerType &)>> events_to_process_;  // +48
    std::vector<ListenerType *> unk3_;                                           // +72
    bool unk4_;                                                                  // +96
    std::thread::id thread_id_;                                                  // +100 (+104)
    bool thread_id_initialized_;                                                 // +104 (+112)
    std::int32_t thread_check_index_;                                            // +112 (+116)
};

class ActorEventCoordinator {
public:
    // void sendEvent(EventRef<ActorGameplayEvent<void>> const &ref);
    // CoordinatorResult sendEvent(EventRef<ActorGameplayEvent<CoordinatorResult>> const &ref);
};

class BlockEventCoordinator {
public:
    // void sendEvent(EventRef<BlockGameplayEvent<void>> const &ref);
    // CoordinatorResult sendEvent(EventRef<BlockGameplayEvent<CoordinatorResult>> const &ref);
};
class ItemEventCoordinator;

class LevelEventCoordinator : public EventCoordinatorPimpl<LevelEventListener> {
public:
    // void sendEvent(EventRef<LevelGameplayEvent<void>> const &ref);
    LevelGameplayHandler &getLevelGameplayHandler();

private:
    std::unique_ptr<LevelGameplayHandler> level_gameplay_handler_;     // +112
    std::shared_ptr<Bedrock::PubSub::SubscriptionBase> subscription_;  // +120
};

class PlayerEventCoordinator {
public:
    ENDSTONE_HOOK void sendEvent(EventRef<PlayerGameplayEvent<void>> const &ref);
    // CoordinatorResult sendEvent(EventRef<PlayerGameplayEvent<CoordinatorResult>> const &ref);
};
class ServerPlayerEventCoordinator : public PlayerEventCoordinator {};
class ClientPlayerEventCoordinator : public PlayerEventCoordinator {};

class ServerInstanceEventCoordinator {
public:
    ENDSTONE_HOOK void sendServerInitializeStart(ServerInstance &instance);
    ENDSTONE_HOOK void sendServerThreadStarted(ServerInstance &instance);
    ENDSTONE_HOOK void sendServerThreadStopped(ServerInstance &instance);
    ENDSTONE_HOOK void sendServerLevelInitialized(ServerInstance &instance, Level &level);
};

class ServerNetworkEventCoordinator;
class ScriptingEventCoordinator;
class ScriptDeferredEventCoordinator;

class ScoreboardEventListener;
class ScoreboardEventCoordinator : public EventCoordinatorPimpl<ScoreboardEventListener> {};
