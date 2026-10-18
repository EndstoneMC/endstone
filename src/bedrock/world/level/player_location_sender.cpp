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

#include "bedrock/world/level/player_location_sender.h"

#include "bedrock/symbol.h"

#ifdef _WIN32
bool PlayerLocationSender::_shouldSendPositionPacket(const Vec3 &viewing_player_position,
                                                     const DimensionType &viewing_player_dimension,
                                                     bool viewing_player_is_spectator,
                                                     const OptionalPosition &observed_player_pos_prev,
                                                     const PlayerLocationData &observed_player_position_new) const
{
    return BEDROCK_CALL(&PlayerLocationSender::_shouldSendPositionPacket, this, viewing_player_position,
                        viewing_player_dimension, viewing_player_is_spectator, observed_player_pos_prev,
                        observed_player_position_new);
}
#elif __linux__
bool PlayerLocationSender::_shouldSendPositionPacket(const Vec3 &viewing_player_position,
                                                     DimensionType viewing_player_dimension,
                                                     bool viewing_player_is_spectator,
                                                     const OptionalPosition &observed_player_pos_prev,
                                                     const PlayerLocationData &observed_player_position_new,
                                                     float simulation_distance)
{
    return BEDROCK_CALL(&PlayerLocationSender::_shouldSendPositionPacket, viewing_player_position,
                        viewing_player_dimension, viewing_player_is_spectator, observed_player_pos_prev,
                        observed_player_position_new, simulation_distance);
}
#endif
