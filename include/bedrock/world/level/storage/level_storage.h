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

#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "bedrock/core/file.h"
#include "bedrock/core/threading.h"
#include "bedrock/forward.h"
#include "bedrock/nbt/compound_tag.h"
#include "bedrock/world/level/storage/db_helpers.h"

class LevelStorage {
public:
    enum class StatsType;

    virtual ~LevelStorage() = 0;
    virtual void addStorageObserver(std::unique_ptr<LevelStorageObserver>) = 0;
    virtual std::unique_ptr<CompoundTag> getCompoundTag(std::string const &, DBHelpers::Category) = 0;
    [[nodiscard]] virtual bool hasKey(std::string_view, DBHelpers::Category) const = 0;
    virtual void forEachKeyWithPrefix(std::string_view, DBHelpers::Category,
                                      std::function<void(std::string_view, std::string_view)> const &) const = 0;
    virtual bool loadLevelData(LevelData &) = 0;
    virtual std::unique_ptr<ChunkSource> createChunkStorage(std::unique_ptr<ChunkSource>, StorageVersion) = 0;
    virtual void saveLevelData(LevelData const &) = 0;
    [[nodiscard]] virtual Core::PathBuffer<std::string> const &getFullPath() const = 0;
    virtual std::shared_ptr<Bedrock::Threading::IAsyncResult<void>> saveData(std::string const &, std::string &&,
                                                                             DBHelpers::Category) = 0;
    virtual std::shared_ptr<Bedrock::Threading::IAsyncResult<void>> saveData(LevelStorageWriteBatch const &) = 0;
    virtual std::shared_ptr<Bedrock::Threading::IAsyncResult<void>> deleteData(std::string const &,
                                                                               DBHelpers::Category) = 0;
    virtual void getStatistics(std::string &, LevelStorage::StatsType) const = 0;
    virtual bool clonePlayerData(std::string_view, std::string_view) = 0;
    [[nodiscard]] virtual Core::LevelStorageResult getLevelStorageState() const = 0;
    virtual void startShutdown() = 0;
    [[nodiscard]] virtual bool isShuttingDown() const = 0;
    virtual bool checkShutdownDone() = 0;
    virtual bool loadData(std::string_view, std::string &, DBHelpers::Category) const = 0;
    [[nodiscard]] virtual Core::LevelStorageResult getState() const = 0;
    virtual std::vector<SnapshotFilenameAndLength> createSnapshot(std::string const &, bool) = 0;
    virtual void releaseSnapshot() = 0;
    virtual std::shared_ptr<Bedrock::Threading::IAsyncResult<void>> compactStorage() = 0;
    virtual void syncAndSuspendStorage() = 0;
    virtual void resumeStorage() = 0;
    virtual void setFlushAllowed(bool) = 0;
    virtual void flushToPermanentStorage() = 0;
    virtual void freeCaches() = 0;
    virtual void setCompactionCallback(std::function<void(CompactionStatus)>) = 0;
    virtual void setCriticalSyncSaveCallback(std::function<void()>) = 0;
    virtual void corruptLevel() = 0;
};
