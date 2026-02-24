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

#include <string>

#include "endstone/event/player/player_event.h"

namespace endstone {

/**
 * @brief Called when a player respawns.
 */
class PlayerRespawnEvent : public PlayerEvent {
public:
    explicit PlayerRespawnEvent(Player &player) : PlayerEvent(player) {}
    ~PlayerRespawnEvent() override = default;

    inline static const std::string NAME = "PlayerRespawnEvent";
    [[nodiscard]] std::string getEventName() const override { return NAME; }
};

}  // namespace endstone
