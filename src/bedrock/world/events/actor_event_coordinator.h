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
#include "bedrock/gameplayhandlers/actor_gameplay_handler.h"
#include "bedrock/world/events/actor_event_listener.h"
#include "bedrock/world/events/event_coordinator.h"

class ActorEventCoordinator : public EventCoordinator<ActorEventListener> {
public:
    std::unique_ptr<ActorGameplayHandler> actor_gameplay_handler;  // Endstone: private -> public

private:
    Bedrock::PubSub::Subscription on_gameplay_user_added_subscription_;
    Bedrock::PubSub::Subscription post_reload_actor_added_subscription_;
};
