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

#include "bedrock/world/level/gameplay_user_manager.h"
#include "bedrock/world/level/level_interface.h"

namespace endstone::core {
class EndstoneServer;
}
class ChunkSource;
class LevelChunk;

class Level : public ILevel {
public:
    ENDSTONE_HOOK void tick() override;
    // ENDSTONE_HOOK void onChunkDiscarded(LevelChunk &);
    // ENDSTONE_HOOK void onChunkLoaded(ChunkSource &, LevelChunk &);

    virtual void *getArmorTrimUnloader() = 0;
    [[nodiscard]] virtual void *getPlayerSleepManager() const = 0;
    virtual void *getPlayerSleepManager() = 0;

#ifdef __linux__  // wtf mojang
    virtual void onSourceCreated(BlockSource &) = 0;
    virtual void onSourceDestroyed(BlockSource &) = 0;
#endif

protected:
    virtual void _subTick() = 0;

public:
    static bool isUsableLevel(const ILevel &level);

protected:
    virtual void _initializeMapDataManager() = 0;
    friend class endstone::core::EndstoneServer;

private:
    ENDSTONE_HOOK void _onChunkLoaded(ChunkSource &, LevelChunk &, int);
};
