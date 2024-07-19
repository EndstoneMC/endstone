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

#include <unordered_map>

#include "bedrock/world/scores/player_scoreboard_id.h"

class IdentityDictionary {
public:
    [[nodiscard]] const ScoreboardId &getScoreboardId(const PlayerScoreboardId &key) const
    {
        auto it = players_.find(key);
        if (it != players_.end()) {
            return it->second;
        }
        return ScoreboardId::INVALID;
    }

    [[nodiscard]] const ScoreboardId &getScoreboardId(const ActorUniqueID &key) const
    {
        auto it = entities_.find(key);
        if (it != entities_.end()) {
            return it->second;
        }
        return ScoreboardId::INVALID;
    }

    [[nodiscard]] const ScoreboardId &getScoreboardId(const std::string &key) const
    {
        auto it = fakes_.find(key);
        if (it != fakes_.end()) {
            return it->second;
        }
        return ScoreboardId::INVALID;
    }

private:
    std::unordered_map<PlayerScoreboardId, ScoreboardId> players_;
    std::unordered_map<ActorUniqueID, ScoreboardId> entities_;
    std::unordered_map<std::string, ScoreboardId> fakes_;
    std::unordered_map<ScoreboardId, IdentityDefinition> identity_defs_;
};
