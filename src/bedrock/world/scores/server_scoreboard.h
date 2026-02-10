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

#include "bedrock/world/level/gameplay_user_manager.h"
#include "bedrock/world/level/storage/level_storage.h"
#include "bedrock/world/scores/scoreboard.h"

class ServerScoreboard : public Scoreboard {
public:
    ServerScoreboard(CommandSoftEnumRegistry, LevelStorage *, Bedrock::NotNullNonOwnerPtr<GameplayUserManager>);

    ~ServerScoreboard() override;
    const DisplayObjective *setDisplayObjective(const std::string &, const Objective &, ObjectiveSortOrder) override;
    Objective *clearDisplayObjective(const std::string &) override;
    const ScoreboardId &createScoreboardId(const Player &) override;
    const ScoreboardId &createScoreboardId(const Actor &) override;
    const ScoreboardId &createScoreboardId(const std::string &) override;
    void onObjectiveAdded(const Objective &) override;
    void onObjectiveRemoved(Objective &) override;
    void onScoreChanged(const ScoreboardId &, const Objective &) override;
    void onPlayerScoreRemoved(const ScoreboardId &, const Objective &) override;
    void onPlayerJoined(const Player &) override;
    void onPlayerIdentityUpdated(const PlayerScoreboardId &) override;
    void tick() override;
    void setPacketSender(PacketSender *sender) override;
    void writeToLevelStorage() override;
    [[nodiscard]] bool isClientSide() const override;

private:
    void *save_timer_;                                                              // +784 (+552)
    LevelStorage *level_storage_;                                                   // +792 (+560)
    bool is_dirty_;                                                                 // +800 (+568)
    Bedrock::NotNullNonOwnerPtr<GameplayUserManager> gameplay_user_manager_;        // +808 (+576)
    std::vector<const Objective *> tracked_objectives_;                             // +816 (+600)
    PacketSender *packet_sender_;                                                   // +832 (+624)
    ScoreboardId last_unique_scoreboard_id_;                                        // +848 (+632)
    std::weak_ptr<void *> on_save_subscription_;                                    // +856 (+648)
    std::weak_ptr<void *> on_level_storage_manager_start_leave_game_subscription_;  // +872 (+664)
    std::weak_ptr<void *> on_remove_actor_entity_references_subscription_;          // +888 (+680)
    std::weak_ptr<void *> on_gameplay_user_removed_subscription_;                   // +904 (+696)
};
BEDROCK_STATIC_ASSERT_SIZE(ServerScoreboard, 928, 696);
