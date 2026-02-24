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

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "endstone/block/block.h"
#include "endstone/event/actor/actor_event.h"
#include "endstone/event/cancellable.h"
#include "endstone/level/location.h"

namespace endstone {

/**
 * @brief Called when an actor explodes
 */
class ActorExplodeEvent : public Cancellable<ActorEvent<Actor>> {
    using BlockList = std::vector<std::unique_ptr<Block>>;

public:
    explicit ActorExplodeEvent(Actor &actor, Location location, BlockList blocks)
        : Cancellable(actor), location_(location), blocks_(std::move(blocks))
    {
    }
    ~ActorExplodeEvent() override = default;

    inline static const std::string NAME = "ActorExplodeEvent";
    [[nodiscard]] std::string getEventName() const override { return NAME; }

    /**
     * @brief Returns the location where the explosion happened.
     * <p>
     * It is not possible to get this value from the Entity as the Entity no longer exists in the world.
     *
     * @return The location of the explosion
     */
    [[nodiscard]] const Location &getLocation() const { return location_; }

    /**
     * @brief Returns the list of blocks that would have been removed or were removed from the explosion event.
     *
     * @return All blown-up blocks
     */
    [[nodiscard]] const BlockList &getBlockList() const { return blocks_; }

    /**
     * @brief Returns the list of blocks that would have been removed or were removed from the explosion event.
     *
     * @return All blown-up blocks
     */
    [[nodiscard]] BlockList &getBlockList() { return blocks_; }

private:
    Location location_;
    BlockList blocks_;
};

}  // namespace endstone
