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

#include "endstone/event/actor/actor_event.h"

namespace endstone {

/**
 * @brief Called when an Actor is spawned into a world.
 *
 * If an Actor Spawn event is cancelled, the actor will not spawn.
 */
class ActorSpawnEvent : public ActorEvent {
public:
    explicit ActorSpawnEvent(Actor &actor) : ActorEvent(actor) {}
    ~ActorSpawnEvent() override = default;

    inline static const std::string NAME = "ActorSpawnEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    [[nodiscard]] bool isCancellable() const override
    {
        return true;
    }

    // TODO(event): add spawn cause
};

}  // namespace endstone
