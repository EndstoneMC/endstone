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

#include "bedrock/forward.h"
#include "bedrock/network/packet_sender.h"
#include "bedrock/world/scores/objective.h"
#include "bedrock/world/scores/objective_sort_order.h"
#include "bedrock/world/scores/scoreboard_id.h"

class Actor;
class Player;

class Scoreboard {
public:
    virtual ~Scoreboard() = 0;
    virtual DisplayObjective const *setDisplayObjective(std::string const &, Objective const &, ObjectiveSortOrder) = 0;
    virtual Objective *clearDisplayObjective(std::string const &) = 0;
    virtual ScoreboardId const &createScoreboardId(Player const &) = 0;
    virtual ScoreboardId const &createScoreboardId(Actor const &) = 0;
    virtual ScoreboardId const &createScoreboardId(std::string const &) = 0;
    virtual void onObjectiveAdded(Objective const &) = 0;
    virtual void onObjectiveRemoved(Objective &) = 0;
    virtual void onScoreChanged(ScoreboardId const &, Objective const &) = 0;
    virtual void onPlayerScoreRemoved(ScoreboardId const &, Objective const &) = 0;
    virtual void onPlayerJoined(Player const &) = 0;
    virtual void onPlayerIdentityUpdated(PlayerScoreboardId const &) = 0;
    virtual void tick() = 0;
    virtual void setPacketSender(PacketSender *sender) = 0;
    virtual void writeToLevelStorage() = 0;
    [[nodiscard]] virtual bool isClientSide() const = 0;

private:
};
