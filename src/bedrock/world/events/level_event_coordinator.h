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

#include "bedrock/core/utility/pub_sub/subscription.h"
#include "bedrock/forward.h"
#include "bedrock/gameplayhandlers/level_gameplay_handler.h"
#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/actor_initialization_method.h"
#include "bedrock/world/events/event_coordinator.h"
#include "bedrock/world/events/level_event_listener.h"

class LevelEventCoordinator : public EventCoordinator<LevelEventListener> {
public:
    std::unique_ptr<LevelGameplayHandler> level_gameplay_handler;  // Endstone: private -> public

private:
    Bedrock::PubSub::Subscription game_rule_change_subscription_;
    Bedrock::PubSub::Subscription on_remove_actor_entity_references_subscription_;
    Bedrock::PubSub::Subscription post_reload_actor_added_subscription_;
    Bedrock::PubSub::Subscription on_gameplay_user_added_subscription_;
};
