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

#include "bedrock/server/commands/standard/teleport_command.h"

#include "endstone/core/symbol.h"

TeleportTarget TeleportCommand::computeTarget(Actor &victim, Vec3 destination, Vec3 *facing_position,
                                              AutomaticID<Dimension, int> destination_dimension,
                                              std::optional<RotationCommandUtils::RotationData> const &rotation_data,
                                              int command_version)
{
    return ENDSTONE_SYMCALL(&TeleportCommand::computeTarget, victim, destination, facing_position,
                            destination_dimension, rotation_data, command_version);
}

void TeleportCommand::applyTarget(Actor &victim, TeleportTarget target, bool keep_velocity)
{
    ENDSTONE_SYMCALL(&TeleportCommand::applyTarget, victim, std::move(target), keep_velocity);
}
