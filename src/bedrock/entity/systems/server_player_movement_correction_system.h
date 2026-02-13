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
#include "bedrock/bedrock.h"
#include "bedrock/entity/components/boat_movement_component.h"
#include "bedrock/entity/components/replay_state_component.h"
#include "bedrock/entity/components/server_player_movement_component.h"
#include "bedrock/entity/components/user_entity_identifier_component.h"
#include "bedrock/entity/strict/strict_entity_context.h"
#include "bedrock/network/packet/player_auth_input_packet.h"
#include "bedrock/world/actor/actor.h"

namespace ServerPlayerMovementCorrectionSystem {
void _afterMovementSimulation(const UserEntityIdentifierComponent &user_identifier, Actor &actor,
                              const PlayerAuthInputPacket &packet, ReplayStateComponent &replay,
                              const ActorRotationComponent *actor_rotation,
                              ServerPlayerMovementComponent &server_player_movement, StateVectorComponent &state_vector,
                              const BoatMovementComponent *boat);
// TODO: hook _tickServerPlayerMovementCorrectionSystem instead
/*
void _tickServerPlayerMovementCorrectionSystem(
    const StrictEntityContext &player,
    const ServerPlayerCurrentMovementComponent &server_player_current_movement_component,
    const UserEntityIdentifierComponent &user_identifier_component,
    ServerPlayerMovementComponent &server_player_movement_component,
    Optional<const PassengerComponent> passengerComponent, StateVectorComponent &player_state_vector,
    const View<const ActorRotationComponent, ActorOwnerComponent, ReplayStateComponent, StateVectorComponent,
               Optional<const BoatMovementComponent>> &replayableEntityView);*/
}  // namespace ServerPlayerMovementCorrectionSystem
