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

#include "endstone/core/block/furnace.h"

namespace endstone::core {

EndstoneFurnace::EndstoneFurnace(const EndstoneBlock &block, ::FurnaceBlockActor &furnace, bool use_snapshot)
    : EndstoneContainerBase<Furnace>(block, furnace, use_snapshot)
{
}

int EndstoneFurnace::getBurnTime() const
{
    return getFurnace().getLitTime();
}

void EndstoneFurnace::setBurnTime(int burn_time)
{
    getFurnace().setLitTime(burn_time);
    getFurnace().setChanged();
}

int EndstoneFurnace::getCookTime() const
{
    return getFurnace().getCookingProgress();
}

void EndstoneFurnace::setCookTime(int cook_time)
{
    getFurnace().setCookingProgress(cook_time);
    getFurnace().setChanged();
}

}  // namespace endstone::core
