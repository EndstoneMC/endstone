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

#include "bedrock/world/actor/provider/actor_offset.h"
#include "endstone/core/entity/components/flag_components.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_jump_event.h"
#include "endstone/event/player/player_move_event.h"
#include "endstone/runtime/hook.h"

// void ServerPlayerMovementCorrectionSystem::_afterMovementSimulation(
//     const UserEntityIdentifierComponent &user_identifier, Actor &actor, const PlayerAuthInputPacket &packet,
//     ReplayStateComponent &replay, const ActorRotationComponent *actor_rotation,
//     ServerPlayerMovementComponent &server_player_movement, StateVectorComponent &state_vector,
//     const BoatMovementComponent *boat)
// {
//     ENDSTONE_HOOK_CALL_ORIGINAL(&ServerPlayerMovementCorrectionSystem::_afterMovementSimulation, user_identifier, actor,
//                                 packet, replay, actor_rotation, server_player_movement, state_vector, boat);
//
//     const auto &server = endstone::core::EndstoneServer::getInstance();
//     if (auto *p = Player::tryGetFromEntity(actor.getEntity()); p) {
//         auto &player = actor.getEndstoneActor<endstone::core::EndstonePlayer>();
//
//         const auto pos = actor.getPosition();
//         const auto pos_prev = actor.getPosPrev();
//         const auto delta = pos - pos_prev;
//
//         const auto rot = actor.getRotation();
//         const auto rot_prev = actor.getRotationPrev();
//         const auto delta_angle = rot - rot_prev;
//
//         const endstone::Location from{player.getDimension(),
//                                       pos_prev.x,
//                                       pos_prev.y - ActorOffset::getHeightOffset(actor.getEntity()),
//                                       pos_prev.z,
//                                       rot_prev.x,
//                                       rot_prev.y};
//         const endstone::Location to = player.getLocation();
//
//         if (packet.getInput(PlayerAuthInputPacket::InputData::Jumping) && p->wasOnGround() && !p->isOnGround() &&
//             delta.y > 0.0F) {
//             endstone::PlayerJumpEvent e{player, from, to};
//             server.getPluginManager().callEvent(e);
//
//             // If the event is cancelled we move the player back to their location before jump.
//             if (e.isCancelled()) {
//                 p->addOrRemoveComponent<endstone::core::InternalTeleportFlagComponent>(true);
//                 player.teleport(e.getFrom());
//                 return;
//             }
//         }
//
//         // Prevent intensive event calls on tiny movement using the thresholds from Spigot
//         if (delta.lengthSquared() > 1.0F / 256 || delta_angle.lengthSquared() > 10.0F) {
//             endstone::PlayerMoveEvent e{player, from, to};
//             server.getPluginManager().callEvent(e);
//
//             // If the event is cancelled we move the player back to their old location.
//             if (e.isCancelled()) {
//                 p->addOrRemoveComponent<endstone::core::InternalTeleportFlagComponent>(true);
//                 player.teleport(e.getFrom());
//                 return;
//             }
//
//             // If a Plugin has changed the To destination then we teleport the Player there
//             if (to != e.getTo()) {
//                 p->addOrRemoveComponent<endstone::core::InternalTeleportFlagComponent>(true);
//                 player.teleport(e.getTo());
//                 return;
//             }
//         }
//     }
// }
