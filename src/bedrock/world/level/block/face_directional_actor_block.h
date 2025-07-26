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

#include "bedrock/world/level/block/actor_block.h"

class FaceDirectionalActorBlock : public ActorBlock {
public:
    FaceDirectionalActorBlock(const std::string &, int, const Material &, bool, float);
    const Block &getRenderBlock() const override;
    FacingID getMappedFace(FacingID, const Block &) const override;
    Flip getFaceFlip(FacingID, const Block &) const override;
    const Block &getPlacementBlock(const Actor &, const BlockPos &, FacingID, const Vec3 &, int) const override;
};
