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

#include "endstone/core/level/chunk.h"

#include <bedrock/world/level/dimension/dimension.h>
#include <endstone/core/server.h>

namespace endstone::core {

EndstoneChunk::EndstoneChunk(const LevelChunk &chunk) : dimension_(chunk.getDimension())
{
    x_ = chunk.getPosition().x;
    z_ = chunk.getPosition().z;
}

int EndstoneChunk::getX() const
{
    return x_;
}

int EndstoneChunk::getZ() const
{
    return z_;
}

Level &EndstoneChunk::getLevel() const
{
    return getDimension().getLevel();
}

Dimension &EndstoneChunk::getDimension() const
{
    return dimension_.getEndstoneDimension();
}

}  // namespace endstone::core
