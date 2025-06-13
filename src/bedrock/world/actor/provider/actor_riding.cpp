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

#include "bedrock/world/actor/provider/actor_riding.h"

#include "bedrock/entity/components/vehicle_component.h"

constexpr std::vector<StrictActorIDEntityContextPair> EMPTY;

const std::vector<StrictActorIDEntityContextPair> &ActorRiding::getPassengers(const EntityContext &entity)
{
    if (auto *vehicle = entity.tryGetComponent<VehicleComponent>(); vehicle) {
        return vehicle->passengers;
    }
    return EMPTY;
}
