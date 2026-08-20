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
#include "bedrock/world/level/block/block_type.h"

#define ENDSTONE_DECLARE_ACTOR_INSIDE_BLOCK(BlockClass)                                              \
    class BlockClass : public BlockType {                                                            \
    public:                                                                                          \
        ENDSTONE_HOOK void entityInside(BlockSource &, BlockPos const &, Actor &) const override;  \
    }

ENDSTONE_DECLARE_ACTOR_INSIDE_BLOCK(BigDripleafBlock);
ENDSTONE_DECLARE_ACTOR_INSIDE_BLOCK(ButtonBlock);
ENDSTONE_DECLARE_ACTOR_INSIDE_BLOCK(DetectorRailBlock);
ENDSTONE_DECLARE_ACTOR_INSIDE_BLOCK(EndPortalBlock);
ENDSTONE_DECLARE_ACTOR_INSIDE_BLOCK(EyeblossomBlock);
ENDSTONE_DECLARE_ACTOR_INSIDE_BLOCK(FrogSpawnBlock);
ENDSTONE_DECLARE_ACTOR_INSIDE_BLOCK(PortalBlock);
ENDSTONE_DECLARE_ACTOR_INSIDE_BLOCK(BasePressurePlateBlock);
ENDSTONE_DECLARE_ACTOR_INSIDE_BLOCK(SweetBerryBushBlock);
ENDSTONE_DECLARE_ACTOR_INSIDE_BLOCK(TripWireBlock);
ENDSTONE_DECLARE_ACTOR_INSIDE_BLOCK(WitherRoseBlock);
ENDSTONE_DECLARE_ACTOR_INSIDE_BLOCK(SoulFireBlock);

#undef ENDSTONE_DECLARE_ACTOR_INSIDE_BLOCK
