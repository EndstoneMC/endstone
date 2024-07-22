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
#include <unordered_map>

#include "bedrock/forward.h"
#include "bedrock/network/packet_sender.h"
#include "bedrock/server/commands/command_soft_enum_registry.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/events/event_coordinator.h"
#include "bedrock/world/scores/display_objective.h"
#include "bedrock/world/scores/identity_definition.h"
#include "bedrock/world/scores/identity_dictionary.h"
#include "bedrock/world/scores/objective.h"
#include "bedrock/world/scores/objective_sort_order.h"
#include "bedrock/world/scores/player_scoreboard_id.h"
#include "bedrock/world/scores/scoreboard_id.h"
#include "bedrock/world/scores/scoreboard_identity_ref.h"

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

    Objective *addObjective(const std::string &, const std::string &, const ObjectiveCriteria &criteria);
    bool removeObjective(Objective *);
    [[nodiscard]] Objective *getObjective(const std::string &name) const;
    [[nodiscard]] const DisplayObjective *getDisplayObjective(const std::string &name) const;
    [[nodiscard]] const ScoreboardId &getScoreboardId(const Player &player) const;
    [[nodiscard]] const ScoreboardId &getScoreboardId(const Actor &actor) const;
    [[nodiscard]] const ScoreboardId &getScoreboardId(const std::string &fake) const;
    [[nodiscard]] bool hasIdentityFor(const ScoreboardId &id) const;
    [[nodiscard]] ScoreboardIdentityRef *getScoreboardIdentityRef(const ScoreboardId &id);
    [[nodiscard]] ObjectiveCriteria *getCriteria(const std::string &name) const;
    void forEachObjective(std::function<void(Objective &)> callback) const;
    void forEachIdentityRef(std::function<void(ScoreboardIdentityRef &)> callback) const;
    void resetPlayerScore(const ScoreboardId &);
    ENDSTONE_HOOK bool resetPlayerScore(const ScoreboardId &, Objective &);
    int modifyPlayerScore(bool &success, const ScoreboardId &, Objective &, int, PlayerScoreSetFunction);

private:
    CommandSoftEnumRegistry registry_;                                                        // +8
    std::unordered_map<std::string, DisplayObjective> display_objectives_;                    // +16
    IdentityDictionary identity_dictionary_;                                                  // +80
    std::unordered_map<ScoreboardId, ScoreboardIdentityRef> identity_refs_;                   // +336 (+216)
    bool should_update_ui_;                                                                   // +400 (+256)
    std::unordered_map<std::string, std::unique_ptr<Objective>> objectives_;                  // +408 (+264)
    std::unordered_map<std::size_t, Bedrock::NonOwnerPointer<Objective>> objectives_lookup_;  // +472 (+304)
    std::unordered_map<std::string, std::unique_ptr<ObjectiveCriteria>> criteria_;            // +536 (+344)
    ScoreboardEventCoordinator scoreboard_event_coordinator_;                                 // +600 (+384)
    // PlayerScoreboardEventListener player_listener_;                                          // +712 (+504)
};
