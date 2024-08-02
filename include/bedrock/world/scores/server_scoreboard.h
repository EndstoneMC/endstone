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

#include "bedrock/world/level/storage/level_storage.h"
#include "bedrock/world/scores/scoreboard.h"

class ServerScoreboard : public Scoreboard {
public:
    static std::unique_ptr<ServerScoreboard> create(Level &level);  // Endstone

private:
    void *save_timer_;                                      // +784 (+552) std::unique_ptr<BasicTimer>
    LevelStorage *level_storage_;                           // +792 (+560)
    bool is_dirty_;                                         // +800 (+568)
    char unknown1_[48];                                     // +808 (+576)
    ScoreboardId last_unique_scoreboard_id_;                // +856 (+624)
    std::weak_ptr<void *> on_save_callback_;                // +872 (+640)
    std::weak_ptr<void *> on_start_level_game_callback_;    // +888 (+656)
    std::weak_ptr<void *> on_remove_actor_callback_;        // +904 (+672)
    std::weak_ptr<void *> gameplay_user_removed_callback_;  // +920 (+688)
};
BEDROCK_STATIC_ASSERT_SIZE(ServerScoreboard, 936, 704);
