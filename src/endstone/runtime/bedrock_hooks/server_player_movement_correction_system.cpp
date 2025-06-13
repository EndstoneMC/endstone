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

#include "bedrock/entity/systems/server_player_movement_correction_system.h"

#include "bedrock/entity/components/depenetration_component.h"
#include "bedrock/entity/systems/actor_set_pos_system.h"
#include "bedrock/network/packet/correct_player_move_prediction_packet.h"
#include "bedrock/network/packet/player_input_tick.h"
#include "bedrock/world/level/level.h"

void ServerPlayerMovementCorrectionSystem::_afterMovementSimulation(
    const UserEntityIdentifierComponent &user_identifier, Actor &actor, const PlayerAuthInputPacket &packet,
    ReplayStateComponent &replay, const ActorRotationComponent *actor_rotation,
    ServerPlayerMovementComponent &server_player_movement, StateVectorComponent &state_vector,
    const BoatMovementComponent *boat)
{
    auto &ctx = actor.getEntity();

    // 1) Check if we should accept the client's position directly
    if (server_player_movement.accept_client_pos_if_within_distance_sq) {
        const Vec3 &server_pos = state_vector.pos;
        const Vec3 &client_pos = packet.pos;
        float dist_sq = (server_pos - client_pos).lengthSquared();

        if (dist_sq <= server_player_movement.accept_client_pos_if_within_distance_sq.value()) {
            ActorSetPosSystem::setPosition(ctx, client_pos, actor.isClientSide());

            auto &dep = ctx.getOrAddComponent<DepenetrationComponent>();
            Vec3 temporary_override = dep.temporary_override.has_value() ? dep.temporary_override.value() : Vec3::ZERO;

            // Clamp per-axis overrides
            temporary_override.x = std::max(temporary_override.x, DepenetrationComponentUtility::TEMP_Y_OVERRIDE.x);
            temporary_override.y = std::max(temporary_override.y, DepenetrationComponentUtility::TEMP_Y_OVERRIDE.y);
            temporary_override.z = std::max(temporary_override.z, DepenetrationComponentUtility::TEMP_Y_OVERRIDE.z);
            dep.temporary_override = temporary_override;

            replay.force_correction = false;
            server_player_movement.accept_client_pos_if_within_distance_sq.reset();
            return;
        }
    }

    // 2) Determine correction strategy
    MovementCorrection correction(CorrectionMethod::None, Vec3::ZERO);
    if (replay.force_correction) {
        correction.method = CorrectionMethod::CorrectClient;  // force full correction
    }
    else {
        const auto &policy = server_player_movement.getPolicy();
        bool strict = policy && policy->isStrictMovement();
        correction = replay.policy->shouldCorrectMovement(ctx, packet, replay.getCurrentTick(),
                                                          replay.getDivergenceCounter(), strict);
    }
    replay.setDivergenceCounter(correction.new_divergence_counter);

    // 3) Apply prediction delta or send correction packet
    if (correction.method == CorrectionMethod::AcceptClient) {
        state_vector.pos_delta += (correction.accept_position - state_vector.pos);
    }
    else if (correction.method == CorrectionMethod::CorrectClient) {
        // Build and send a correction packet
        CorrectPlayerMovePredictionPacket pk;
        pk.pos = state_vector.pos;
        pk.pos_delta = state_vector.pos_delta;

        // If not a player, include vehicle/boat data
        if (!actor.isPlayer()) {
            if (actor_rotation) {
                pk.rotation = actor_rotation->rotation_degree;
            }
            if (boat) {
                pk.vehicle_angular_velocity = boat->y_rot_degree;
            }
        }

        pk.tick = PlayerInputTick(replay.getCurrentTick());
        pk.on_ground = actor.isOnGround();
        pk.prediction_type = actor.isPlayer() ? RewindType::Player : RewindType::Vehicle;

        actor.getLevel().getPacketSender()->sendToClient(&user_identifier, pk);
    }

    // 4) Clear force-correction flag and accept-distance
    replay.force_correction = false;
    server_player_movement.accept_client_pos_if_within_distance_sq.reset();
}
