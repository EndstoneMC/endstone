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

#include "bedrock/world/scores/server_scoreboard.h"

#include "bedrock/symbol.h"

namespace {
[[noreturn]] void unreachable()
{
    throw std::runtime_error("unreachable");
}
}  // namespace

ServerScoreboard::ServerScoreboard(CommandSoftEnumRegistry registry, LevelStorage *storage,
                                   Bedrock::NotNullNonOwnerPtr<GameplayUserManager> gameplay_user_manager)
    : gameplay_user_manager_(gameplay_user_manager)
{
    BEDROCK_CTOR(ServerScoreboard, CommandSoftEnumRegistry, LevelStorage *,
                 Bedrock::NotNullNonOwnerPtr<GameplayUserManager>)
    (this, registry, storage, gameplay_user_manager);
}

ServerScoreboard::~ServerScoreboard() = default;

const DisplayObjective *ServerScoreboard::setDisplayObjective(const std::string &, const Objective &,
                                                              ObjectiveSortOrder)
{
    unreachable();
}

Objective *ServerScoreboard::clearDisplayObjective(const std::string &)
{
    unreachable();
}

const ScoreboardId &ServerScoreboard::createScoreboardId(const Player &)
{
    unreachable();
}

const ScoreboardId &ServerScoreboard::createScoreboardId(const Actor &)
{
    unreachable();
}

const ScoreboardId &ServerScoreboard::createScoreboardId(const std::string &)
{
    unreachable();
}

void ServerScoreboard::onObjectiveAdded(const Objective &)
{
    unreachable();
}

void ServerScoreboard::onObjectiveRemoved(Objective &)
{
    unreachable();
}

void ServerScoreboard::onScoreChanged(const ScoreboardId &, const Objective &)
{
    unreachable();
}

void ServerScoreboard::onPlayerScoreRemoved(const ScoreboardId &, const Objective &)
{
    unreachable();
}

void ServerScoreboard::onPlayerJoined(const Player &)
{
    unreachable();
}

void ServerScoreboard::onPlayerIdentityUpdated(const PlayerScoreboardId &)
{
    unreachable();
}

void ServerScoreboard::tick()
{
    unreachable();
}

void ServerScoreboard::setPacketSender(PacketSender *sender)
{
    unreachable();
}

void ServerScoreboard::writeToLevelStorage()
{
    unreachable();
}

bool ServerScoreboard::isClientSide() const
{
    unreachable();
}
