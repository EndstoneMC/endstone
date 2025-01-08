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

#include "bedrock/symbol.h"
#include "bedrock/world/actor/player/player.h"

Scoreboard::~Scoreboard() = default;

Objective *Scoreboard::addObjective(const std::string &name, const std::string &display_name,
                                    const ObjectiveCriteria &criteria)
{
    if (getObjective(name)) {
        return nullptr;  // already exist
    }

    auto objective = std::make_unique<Objective>(name, criteria);
    objectives_[name] = std::move(objective);
    auto &ref = *objectives_[name];
    objectives_lookup_.emplace(HashedString::computeHash(name), ref);
    ref.setDisplayName(display_name);
    onObjectiveAdded(ref);
    return &ref;
}

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
        if (&it->second.getObjective() == objective) {
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

ObjectiveCriteria *Scoreboard::getCriteria(const std::string &name) const
{
    auto it = criteria_.find(name);
    if (it != criteria_.end()) {
        return it->second.get();
    }
    return nullptr;
}

void Scoreboard::forEachObjective(std::function<void(Objective &)> callback) const
{
    for (const auto &[key, value] : objectives_) {
        callback(*value);
    }
}

void Scoreboard::forEachIdentityRef(std::function<void(ScoreboardIdentityRef &)> callback) const
{
    for (const auto &[key, value] : identity_refs_) {
        callback(const_cast<ScoreboardIdentityRef &>(value));
    }
}

void Scoreboard::resetPlayerScore(const ScoreboardId &id)
{
    for (auto &[key, value] : objectives_) {
        resetPlayerScore(id, *value);
    }
}

bool Scoreboard::resetPlayerScore(const ScoreboardId &id, Objective &objective)
{
    bool (Scoreboard::*func)(const ScoreboardId &, Objective &) = &Scoreboard::resetPlayerScore;
    ENDSTONE_SYMCALL(func, this, id, objective);
}

int Scoreboard::modifyPlayerScore(bool &success, const ScoreboardId &id, Objective &objective, int score,
                                  PlayerScoreSetFunction action)
{
    int result = 0;
    auto *id_ref = getScoreboardIdentityRef(id);
    if (!id_ref) {
        success = false;
        return result;
    }

    success = id_ref->modifyScoreInObjective(result, objective, score, action);
    if (success) {
        onScoreChanged(id, objective);
    }
    return result;
}
