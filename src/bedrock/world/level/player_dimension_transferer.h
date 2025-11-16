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

#include "bedrock/forward.h"

struct AddLimboActorHelper;

class IPlayerDimensionTransferConnector {
public:
    virtual Bedrock::PubSub::Connector<void(AutomaticID<Dimension, int>)> &
    getOnAnyPlayerChangeDimensionPreSuspendRegionConnector() = 0;
    virtual Bedrock::PubSub::Connector<void()> &getOnAnyPlayerChangeDimensionPrepareRegionCompleteConnector() = 0;
};

class IPlayerDimensionTransferer : public IPlayerDimensionTransferConnector {
public:
    virtual ~IPlayerDimensionTransferer() = default;
    virtual void playerSaveLimboActors(Player &, ChangeDimensionRequest &, Dimension &, PlayerLimboActorManager *) = 0;
    virtual void playerDestroyRemotePlayers(GameplayUserManager &) = 0;
    virtual void playerStartChangeDimensionSuspendRegion(Player &, DimensionType) = 0;
    virtual void setTransitionLocation(Player &, ChangeDimensionRequest &, Dimension &) = 0;
    virtual void syncTransitionComponentTargetPosition(Player &, const ChangeDimensionRequest &) = 0;
    virtual void playerDestroyRegion(Player &, const ChangeDimensionRequest &) = 0;
    virtual void playerPrepareRegion(Player &, const ChangeDimensionRequest &, const Dimension &) = 0;
    virtual bool playerWaitForServer(Player &, std::chrono::steady_clock::time_point) = 0;
    virtual bool playerWaitForDimensionTransitionSystem(const Player &, EntityRegistry &) = 0;
    virtual void sendClientRespawnMovePacketFromServer(const Player &, ChangeDimensionRequest &) = 0;
    virtual bool waitForSubChunks(Player &, const Dimension &) = 0;
    virtual void startWaitForRespawn(const Player &) = 0;
    virtual void playerSwitchDimension(Player &, ChangeDimensionRequest &, AddLimboActorHelper *) = 0;
    virtual bool doRespawnIfReady(Player &, ChangeDimensionRequest &, AddLimboActorHelper *) = 0;
    virtual void setPacketSender(PacketSender &) = 0;
};
