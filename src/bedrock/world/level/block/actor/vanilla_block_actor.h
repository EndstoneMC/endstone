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

#include "bedrock/world/level/block/actor/block_actor.h"

class BlockSource;

class IVanillaMainBlockActorComponent {
public:
    virtual ~IVanillaMainBlockActorComponent() = default;
};

class IVanillaRenderBlockActorComponent {
public:
    virtual ~IVanillaRenderBlockActorComponent() = default;
};

class IVanillaTickBlockActorComponent {
public:
    virtual ~IVanillaTickBlockActorComponent() = default;
};

class VanillaBlockActor : public BlockActor,
                          public IVanillaMainBlockActorComponent,
                          public IVanillaRenderBlockActorComponent,
                          public IVanillaTickBlockActorComponent {
public:
    virtual void tick(BlockSource &) = 0;

    void setChanged() { properties_.insert(BlockActor::Property::Changed); }
    [[nodiscard]] bool isChanged() const { return properties_.contains(BlockActor::Property::Changed); }

protected:
    int tick_count_;                                    // +56
    BlockActor::Properties properties_;                 // +60
    AABB bb_;                                           // +64
    BlockActorRendererId renderer_id_;                  // +88
    int repair_cost_;                                   // +92
    ActorTerrainInterlockData terrain_interlock_data_;  // +96
    Bedrock::Safety::RedactableString custom_name_;     // +120
    std::string filtered_custom_name_;                  // +176
};
