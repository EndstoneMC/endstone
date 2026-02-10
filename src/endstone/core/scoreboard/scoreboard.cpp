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

#include "endstone/core/scoreboard/scoreboard.h"

#include <stdexcept>

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/scores/objective_criteria.h"
#include "bedrock/world/scores/scoreboard.h"
#include "endstone/core/actor/actor.h"
#include "endstone/core/level/level.h"
#include "endstone/core/player.h"
#include "endstone/core/scoreboard/objective.h"
#include "endstone/core/scoreboard/score.h"
#include "endstone/core/server.h"

namespace endstone::core {

EndstoneScoreboard::EndstoneScoreboard(::Scoreboard &board) : board_(board)
{
    init();
}

EndstoneScoreboard::EndstoneScoreboard(std::unique_ptr<::Scoreboard> board) : board_(*board)
{
    holder_ = std::move(board);
    init();
}

void EndstoneScoreboard::init()
{
    auto &server = EndstoneServer::getInstance();
    const auto *level = server.getEndstoneLevel();
    packet_sender_ = std::make_unique<ScoreboardPacketSender>(server, *this, *level->getHandle().getPacketSender());
    board_.setPacketSender(packet_sender_.get());
}

std::unique_ptr<Objective> EndstoneScoreboard::addObjective(std::string name, Criteria::Type criteria)
{
    return addObjective(name, criteria, name);
}

std::unique_ptr<Objective> EndstoneScoreboard::addObjective(std::string name, Criteria::Type criteria,
                                                            std::string display_name)
{
    return addObjective(name, criteria, name, RenderType::Integer);
}

std::unique_ptr<Objective> EndstoneScoreboard::addObjective(std::string name, Criteria::Type criteria,
                                                            std::string display_name, RenderType render_type)
{
    const auto *cr = board_.getCriteria(getCriteriaName(criteria));
    Preconditions::checkState(cr != nullptr, "Criteria does not exist.");
    auto *objective = board_.addObjective(name, display_name, *cr);
    Preconditions::checkState(objective != nullptr, "Objective {} already exists.", name);
    return std::make_unique<EndstoneObjective>(*this, *objective);
}

std::unique_ptr<Objective> EndstoneScoreboard::getObjective(std::string name) const
{
    if (auto *objective = board_.getObjective(name); objective) {
        return std::make_unique<EndstoneObjective>(const_cast<EndstoneScoreboard &>(*this), *objective);
    }
    return nullptr;
}

std::unique_ptr<Objective> EndstoneScoreboard::getObjective(DisplaySlot slot) const
{
    const auto *display_objective = board_.getDisplayObjective(toMinecraftSlot(slot));
    if (!display_objective) {
        return nullptr;
    }
    return std::make_unique<EndstoneObjective>(const_cast<EndstoneScoreboard &>(*this),
                                               const_cast<::Objective &>(display_objective->getObjective()));
}

std::vector<std::unique_ptr<Objective>> EndstoneScoreboard::getObjectives() const
{
    std::vector<std::unique_ptr<Objective>> result;
    board_.forEachObjective([&](auto &objective) {
        result.push_back(std::make_unique<EndstoneObjective>(const_cast<EndstoneScoreboard &>(*this), objective));
    });
    return result;
}

std::vector<std::unique_ptr<Objective>> EndstoneScoreboard::getObjectivesByCriteria(Criteria::Type criteria) const
{
    std::vector<std::unique_ptr<Objective>> result;
    board_.forEachObjective([&](auto &objective) {
        if (objective.getCriteria().getName() == getCriteriaName(criteria)) {
            result.push_back(std::make_unique<EndstoneObjective>(const_cast<EndstoneScoreboard &>(*this), objective));
        }
    });
    return result;
}

std::vector<std::unique_ptr<Score>> EndstoneScoreboard::getScores(ScoreEntry entry) const
{
    std::vector<std::unique_ptr<Score>> result;
    board_.forEachObjective([&](auto &objective) {
        auto obj = std::make_unique<EndstoneObjective>(const_cast<EndstoneScoreboard &>(*this), objective);
        result.push_back(std::make_unique<EndstoneScore>(std::move(obj), entry));
    });
    return result;
}

void EndstoneScoreboard::resetScores(ScoreEntry entry)
{
    const auto &scoreboard_id = getScoreboardId(entry);
    if (scoreboard_id.isValid()) {
        board_.resetPlayerScore(getScoreboardId(entry));
    }
}

std::vector<ScoreEntry> EndstoneScoreboard::getEntries() const
{
    std::vector<ScoreEntry> result;
    auto &server = entt::locator<EndstoneServer>::value();
    board_.forEachIdentityRef([&](auto &id_ref) {
        switch (id_ref.getIdentityType()) {
        case IdentityDefinition::Type::Player: {
            auto players = server.getOnlinePlayers();
            for (const auto &player : players) {
                if (static_cast<EndstonePlayer *>(player)->getHandle().getOrCreateUniqueID() ==
                    id_ref.getPlayerId().actor_unique_id) {
                    result.emplace_back(player);
                }
            }
            break;
        }
        case IdentityDefinition::Type::Entity: {
            auto actors = server.getLevel()->getActors();
            for (const auto &actor : actors) {
                if (ActorUniqueID(actor->getId()) == id_ref.getEntityId()) {
                    result.emplace_back(actor);
                }
            }
            break;
        }
        case IdentityDefinition::Type::FakePlayer: {
            result.push_back(id_ref.getFakePlayerName());
            break;
        }
        default:
            throw std::runtime_error("Invalid IdentityDefinition::Type");
        }
    });
    return result;
}

void EndstoneScoreboard::clearSlot(DisplaySlot slot)
{
    board_.clearDisplayObjective(toMinecraftSlot(slot));
}

std::string EndstoneScoreboard::getCriteriaName(Criteria::Type type)
{
    switch (type) {
    case Criteria::Type::Dummy:
        return "dummy";
    default:
        throw std::runtime_error("Unknown Criteria::Type!");
    }
}

std::string EndstoneScoreboard::toMinecraftSlot(DisplaySlot slot)
{
    switch (slot) {
    case DisplaySlot::BelowName:
        return "belowname";
    case DisplaySlot::PlayerList:
        return "list";
    case DisplaySlot::SideBar:
        return "sidebar";
    default:
        throw std::runtime_error("Unknown DisplaySlot!");
    }
}
DisplaySlot EndstoneScoreboard::fromMinecraftSlot(std::string slot)
{
    static std::unordered_map<std::string, DisplaySlot> lookup_map = {
        {::Scoreboard::DISPLAY_SLOT_LIST, DisplaySlot::PlayerList},
        {::Scoreboard::DISPLAY_SLOT_SIDEBAR, DisplaySlot::SideBar},
        {::Scoreboard::DISPLAY_SLOT_BELOWNAME, DisplaySlot::BelowName},
    };
    return lookup_map.at(slot);
}

const ::ScoreboardId &EndstoneScoreboard::getScoreboardId(ScoreEntry entry) const
{
    return std::visit(overloaded{[&](Player *player) -> const ::ScoreboardId & {
                                     if (!player) {
                                         return ScoreboardId::INVALID;
                                     }
                                     return board_.getScoreboardId(static_cast<EndstonePlayer *>(player)->getHandle());
                                 },
                                 [&](Actor *actor) -> const ::ScoreboardId & {
                                     if (!actor) {
                                         return ScoreboardId::INVALID;
                                     }
                                     return board_.getScoreboardId(static_cast<EndstoneActor *>(actor)->getHandle());
                                 },
                                 [&](const std::string &fake) -> const ::ScoreboardId & {
                                     return board_.getScoreboardId(fake);
                                 }},
                      entry);
}

const ::ScoreboardId &EndstoneScoreboard::getOrCreateScoreboardId(ScoreEntry entry)
{
    const auto &scoreboard_id = getScoreboardId(entry);
    if (scoreboard_id.isValid()) {
        return scoreboard_id;
    }

    return std::visit(overloaded{[&](Player *player) -> const ::ScoreboardId & {
                                     if (!player) {
                                         return ScoreboardId::INVALID;
                                     }
                                     return board_.createScoreboardId(
                                         static_cast<EndstonePlayer *>(player)->getHandle());
                                 },
                                 [&](Actor *actor) -> const ::ScoreboardId & {
                                     if (!actor) {
                                         return ScoreboardId::INVALID;
                                     }
                                     return board_.createScoreboardId(static_cast<EndstoneActor *>(actor)->getHandle());
                                 },
                                 [&](const std::string &fake) -> const ::ScoreboardId & {
                                     return board_.createScoreboardId(fake);
                                 }},
                      entry);
}

::Scoreboard &EndstoneScoreboard::getHandle() const
{
    return board_;
}

}  // namespace endstone::core
