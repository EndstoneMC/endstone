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

#include "bedrock/world/scores/scoreboard.h"

#include "bedrock/world/actor/player/player.h"

bool Scoreboard::removeObjective(Objective *objective)
{
    if (!objective) {
        return false;
    }

    const auto &name = objective->getName();
    {
        auto it = objectives_.find(name);
        if (it == objectives_.end()) {
            return false;
        }
        onObjectiveRemoved(*objective);
        objectives_.erase(it);
    }

    for (auto it = display_objectives_.begin(); it != display_objectives_.end();) {
        if (it->second.getObjective() == objective) {
            it = display_objectives_.erase(it);
            continue;
        }
        ++it;
    }
    return true;
}

Objective *Scoreboard::getObjective(const std::string &name) const
{
    auto it = objectives_.find(name);
    if (it == objectives_.end()) {
        return nullptr;
    }
    return it->second.get();
}

const DisplayObjective *Scoreboard::getDisplayObjective(const std::string &name) const
{
    auto it = display_objectives_.find(name);
    if (it == display_objectives_.end()) {
        return nullptr;
    }
    return &it->second;
}

const ScoreboardId &Scoreboard::getScoreboardId(const Player &player) const
{
    return identity_dictionary_.getScoreboardId(PlayerScoreboardId{player.getOrCreateUniqueID()});
}

const ScoreboardId &Scoreboard::getScoreboardId(const Actor &actor) const
{
    if (actor.hasCategory(ActorCategory::Player)) {
        return getScoreboardId(static_cast<const Player &>(actor));
    }
    return identity_dictionary_.getScoreboardId(actor.getOrCreateUniqueID());
}

const ScoreboardId &Scoreboard::getScoreboardId(const std::string &fake) const
{
    auto &id = identity_dictionary_.getScoreboardId(fake);
    if (id.isValid()) {
        return id;
    }
    try {
        ActorUniqueID actor_unique_id;
        actor_unique_id.raw_id = std::stoll(fake);
        return identity_dictionary_.getScoreboardId(actor_unique_id);
    }
    catch (std::exception &) {
        return id;
    }
}

bool Scoreboard::hasIdentityFor(const ScoreboardId &id) const
{
    return identity_refs_.find(id) != identity_refs_.end();
}

ScoreboardIdentityRef *Scoreboard::getScoreboardIdentityRef(const ScoreboardId &id)
{
    auto it = identity_refs_.find(id);
    if (it != identity_refs_.end()) {
        return &it->second;
    }
    return nullptr;
}
