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
#include "endstone/detail/utility.h"

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
    endstone::detail::unreachable();
}
Objective *ServerScoreboard::clearDisplayObjective(const std::string &)
{
    endstone::detail::unreachable();
}
const ScoreboardId &ServerScoreboard::createScoreboardId(const Player &)
{
    endstone::detail::unreachable();
}
const ScoreboardId &ServerScoreboard::createScoreboardId(const Actor &)
{
    endstone::detail::unreachable();
}
const ScoreboardId &ServerScoreboard::createScoreboardId(const std::string &)
{
    endstone::detail::unreachable();
}
void ServerScoreboard::onObjectiveAdded(const Objective &)
{
    endstone::detail::unreachable();
}
void ServerScoreboard::onObjectiveRemoved(Objective &)
{
    endstone::detail::unreachable();
}
void ServerScoreboard::onScoreChanged(const ScoreboardId &, const Objective &)
{
    endstone::detail::unreachable();
}
void ServerScoreboard::onPlayerScoreRemoved(const ScoreboardId &, const Objective &)
{
    endstone::detail::unreachable();
}
void ServerScoreboard::onPlayerJoined(const Player &)
{
    endstone::detail::unreachable();
}
void ServerScoreboard::onPlayerIdentityUpdated(const PlayerScoreboardId &)
{
    endstone::detail::unreachable();
}
void ServerScoreboard::tick()
{
    endstone::detail::unreachable();
}
void ServerScoreboard::setPacketSender(PacketSender *sender)
{
    endstone::detail::unreachable();
}
void ServerScoreboard::writeToLevelStorage()
{
    endstone::detail::unreachable();
}
bool ServerScoreboard::isClientSide() const
{
    endstone::detail::unreachable();
}
