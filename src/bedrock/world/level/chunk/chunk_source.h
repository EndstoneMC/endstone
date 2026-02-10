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

#include "bedrock/core/utility/enable_non_owner_references.h"
#include "bedrock/core/utility/pub_sub/subscription.h"
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/chunk/chunk_source_lookup_map.h"
#include "bedrock/world/scores/identity_dictionary.h"

struct ChunkDeletionMetadata {
    std::atomic<int> delete_count;
};

class ChunkSource : public Bedrock::EnableNonOwnerReferences {
public:
    enum class LoadMode : int {
        None = 0,
        Deferred = 1,
    };

    ChunkSource(Dimension *, int);

    virtual ~ChunkSource() = 0;
    virtual void shutdown() = 0;
    virtual bool isShutdownDone() = 0;
    virtual std::shared_ptr<LevelChunk> getExistingChunk(const ChunkPos &) = 0;
    virtual std::shared_ptr<LevelChunk> getRandomChunk(Random &) = 0;
    virtual bool isChunkKnown(const ChunkPos &) = 0;
    virtual bool isChunkSaved(const ChunkPos &) = 0;
    virtual std::shared_ptr<LevelChunk> createNewChunk(const ChunkPos &cp, LoadMode lm, bool read_only) = 0;
    virtual std::shared_ptr<LevelChunk> getOrLoadChunk(const ChunkPos &cp, LoadMode lm, bool read_only) = 0;
    virtual bool structurePostProcessChunk(ChunkViewSource &);
    virtual bool decorationPostProcessChunk(ChunkViewSource &);
    virtual void checkAndReplaceChunk(ChunkViewSource &neighborhood, LevelChunk &lc);
    virtual bool verifyChunkNeedsNeighborAwareUpgrade(LevelChunk &);
    virtual void neighborAwareChunkUpgrade(LevelChunk &, ChunkViewSource &);
    virtual void loadChunk(LevelChunk &, bool) = 0;
    virtual void postProcessMobsAt(BlockSource &, int, int, Random &) = 0;
    virtual void postProcessMobsAt(BlockSource &, const BoundingBox &) const = 0;
    virtual void deleteAllChunkData(std::unordered_set<ChunkPos> chunks_to_delete,
                                    std::function<void()> completion_callback,
                                    std::shared_ptr<ChunkDeletionMetadata> metadata);
    virtual void deleteStoredChunkData(std::unordered_set<ChunkPos> chunks_to_delete,
                                       std::function<void()> completion_callback,
                                       std::shared_ptr<ChunkDeletionMetadata> metadata);
    virtual bool saveLiveChunk(LevelChunk &) = 0;
    virtual void writeEntityChunkTransfer(LevelChunk &) = 0;
    virtual void writeEntityChunkTransfersToUnloadedChunk(const ChunkKey &,
                                                          const std::vector<ActorUnloadedChunkTransferEntry> &) = 0;
    virtual void deserializeActorStorageToLevelChunk(LevelChunk &) = 0;
    virtual void hintDiscardBatchBegin() = 0;
    virtual void hintDiscardBatchEnd() = 0;
    virtual void acquireDiscarded(TrackedUniqueChunkPtr) = 0;
    virtual void compact() = 0;
    virtual void flushPendingDiscardedChunkWrites() = 0;
    virtual void flushThreadBatch() = 0;
    virtual bool isWithinWorldLimit(const ChunkPos &) const = 0;
    virtual const ChunkSourceLookupMap *getChunkMap() = 0;
    virtual const ChunkSourceLookupMap &getStorage() const = 0;
    virtual void clearDeletedEntities() = 0;
    virtual bool canCreateViews() const = 0;
    virtual std::unique_ptr<BlendingDataProvider> tryGetBlendingDataProvider() = 0;
    virtual std::shared_ptr<LevelChunkMetaDataDictionary> loadLevelChunkMetaDataDictionary() = 0;
    virtual void setLevelChunk(std::shared_ptr<LevelChunk>) = 0;
    virtual bool canLaunchTasks() const = 0;
    virtual bool chunkPosNeedsBlending(const ChunkPos &) = 0;

private:
    int chunk_side_;
    Level *level_;
    Dimension *dimension_;
    ChunkSource *parent_;
    std::unique_ptr<ChunkSource> owned_parent_;
    LevelChunkBuilderData *level_chunk_builder_data_;
    std::atomic<bool> shutting_down_;
    Bedrock::PubSub::Subscription on_save_subscription_;
    Bedrock::PubSub::Subscription on_level_storage_app_suspend_subscription_;
};
