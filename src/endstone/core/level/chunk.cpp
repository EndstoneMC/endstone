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

#include <stdexcept>

#include "bedrock/world/level/dimension/dimension.h"
#include "endstone/core/server.h"
#include "endstone/level/dimension.h"

namespace endstone::core {

EndstoneChunk::EndstoneChunk(WeakRef<::Dimension> dimension, int x, int z)
    : dimension_(std::move(dimension)), x_(x), z_(z)
{
}

EndstoneChunk::EndstoneChunk(const LevelChunk &chunk)
    : EndstoneChunk(chunk.getDimension().getWeakRef(), chunk.getPosition().x, chunk.getPosition().z)
{
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
    return getDimension()->getLevel();
}

NotNull<Dimension> EndstoneChunk::getDimension() const
{
    const auto handle = dimension_.unwrap();
    if (!handle) {
        throw std::runtime_error("Trying to access a dimension that is no longer valid.");
    }
    return handle->getEndstoneDimension();
}

bool EndstoneChunk::isLoaded() const
{
    return getDimension()->isChunkLoaded(x_, z_);
}

bool EndstoneChunk::load()
{
    return getDimension()->loadChunk(x_, z_);
}

bool EndstoneChunk::load(bool generate)
{
    return getDimension()->loadChunk(x_, z_, generate);
}

bool EndstoneChunk::unload()
{
    return getDimension()->unloadChunk(x_, z_);
}

bool EndstoneChunk::addPluginChunkTicket(Plugin &plugin)
{
    return getDimension()->addPluginChunkTicket(x_, z_, plugin);
}

bool EndstoneChunk::removePluginChunkTicket(Plugin &plugin)
{
    return getDimension()->removePluginChunkTicket(x_, z_, plugin);
}

std::vector<Plugin *> EndstoneChunk::getPluginChunkTickets() const
{
    return getDimension()->getPluginChunkTickets(x_, z_);
}

}  // namespace endstone::core
