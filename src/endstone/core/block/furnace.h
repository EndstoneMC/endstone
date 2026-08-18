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

#include "bedrock/world/level/block/actor/furnace_block_actor.h"
#include "endstone/block/furnace.h"
#include "endstone/core/block/container.h"

namespace endstone::core {

class EndstoneFurnace : public EndstoneContainerBase<Furnace> {
public:
    EndstoneFurnace(const EndstoneBlock &block, ::FurnaceBlockActor &furnace, bool use_snapshot);

    [[nodiscard]] int getBurnTime() const override;
    void setBurnTime(int burn_time) override;
    [[nodiscard]] int getCookTime() const override;
    void setCookTime(int cook_time) override;

private:
    [[nodiscard]] ::FurnaceBlockActor &getFurnace() const { return getBlockActor<::FurnaceBlockActor>(); }
};

}  // namespace endstone::core
