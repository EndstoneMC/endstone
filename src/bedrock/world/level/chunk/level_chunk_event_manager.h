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

#include "bedrock/core/utility/pub_sub/connector.h"
#include "bedrock/core/utility/pub_sub/publisher.h"
#include "bedrock/forward.h"

class ILevelChunkEventManagerConnector {
public:
    virtual ~ILevelChunkEventManagerConnector();
    virtual Bedrock::PubSub::Connector<void(ChunkSource &, LevelChunk &, int)> &getOnChunkLoadedConnector() = 0;
    virtual Bedrock::PubSub::Connector<void(ChunkSource &, LevelChunk &)> &getOnChunkReloadedConnector() = 0;
    virtual Bedrock::PubSub::Connector<void(LevelChunk &)> &getOnChunkDiscardedConnector() = 0;
};
static_assert(sizeof(ILevelChunkEventManagerConnector) == 8);

class LevelChunkEventManager : public ILevelChunkEventManagerConnector {
public:
    LevelChunkEventManager(std::unique_ptr<ILevelChunkEventManagerProxy>);

private:
    Bedrock::PubSub::Publisher<void(ChunkSource &, LevelChunk &, int), Bedrock::PubSub::ThreadModel::MultiThreaded>
        on_chunk_loaded_publisher_;
    Bedrock::PubSub::Publisher<void(ChunkSource &, LevelChunk &), Bedrock::PubSub::ThreadModel::MultiThreaded>
        on_chunk_reloaded_publisher_;
    Bedrock::PubSub::Publisher<void(LevelChunk &), Bedrock::PubSub::ThreadModel::MultiThreaded>
        on_chunk_discarded_publisher_;
    const gsl::not_null<std::unique_ptr<ILevelChunkEventManagerProxy>> level_chunk_event_manager_proxy;
};
