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

#include <gsl/gsl>

#include "bedrock/gamerefs/stack_ref_result.h"
#include "bedrock/world/level/gameplay_user_manager.h"
#include "bedrock/world/level/storage/level_storage.h"
#include "bedrock/world/scores/scoreboard.h"
#include "endstone/detail/hook.h"

class ServerScoreboard : public Scoreboard {
public:
    ENDSTONE_FACTORY_DECLARE(ServerScoreboard, CommandSoftEnumRegistry, LevelStorage *,
                             Bedrock::NotNullNonOwnerPtr<GameplayUserManager>);

private:
    void *save_timer_;                                                              // +792 (+552)
    LevelStorage *level_storage_;                                                   // +800 (+560)
    bool is_dirty_;                                                                 // +808 (+568)
    Bedrock::NotNullNonOwnerPtr<GameplayUserManager> gameplay_user_manager_;        // +816 (+576)
    std::vector<const Objective *> tracked_objectives_;                             // +832 (+592)
    PacketSender *packet_sender_;                                                   // +848 (+616)
    ScoreboardId last_unique_scoreboard_id_;                                        // +856 (+624)
    std::weak_ptr<void *> on_save_subscription_;                                    // +872 (+640)
    std::weak_ptr<void *> on_level_storage_manager_start_leave_game_subscription_;  // +888 (+656)
    std::weak_ptr<void *> on_remove_actor_entity_references_subscription_;          // +904 (+672)
    std::weak_ptr<void *> on_gameplay_user_removed_subscription_;                   // +920 (+688)
    std::shared_ptr<void *> scoreboard_imgui_module_;                               //
};
BEDROCK_STATIC_ASSERT_SIZE(ServerScoreboard, 968, 736);
