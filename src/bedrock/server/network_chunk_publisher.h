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

#include <chrono>
#include <cstdint>
#include <functional>
#include <memory>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/common_types.h"
#include "bedrock/core/math/vec3.h"
#include "bedrock/forward.h"
#include "bedrock/network/network_identifier.h"
#include "bedrock/util/grid_area.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/chunk_pos.h"
#include "bedrock/world/level/dimension/dimension_type.h"

class ChunkSource;
class ILevel;
class LevelChunk;
class ServerNetworkSystem;

struct ChunkPositionAndDimension {
    ChunkPos pos;
    DimensionType type;
    bool operator==(const ChunkPositionAndDimension &rhs) const
    {
        return pos.x == rhs.pos.x && pos.z == rhs.pos.z && type == rhs.type;
    }
};

namespace std {
template <>
struct hash<ChunkPositionAndDimension> {  // NOLINT
    std::size_t operator()(const ChunkPositionAndDimension &key) const
    {
        std::hash<int> hasher;
        std::size_t seed = 0;
        seed ^= hasher(key.pos.x) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hasher(key.pos.z) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hasher(key.type.value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
        return seed;
    }
};
}  // namespace std

struct ClientGenerationRequestHandler {
    std::vector<std::pair<ChunkPos, std::shared_ptr<LevelChunk>>> server_chunks;
    ChunkPos center;
    int chunk_radius;
    Vec3 facing_direction;
    std::uint8_t paused_ticks;
    static const std::uint8_t WAIT_TICKS = 3;
};

class NetworkChunkPublisher {
public:
    NetworkChunkPublisher(ILevel &level, const NetworkIdentifier &owner, SubClientId sub_client_id);
    virtual ~NetworkChunkPublisher();

public:  // Endstone: private -> public
    ILevel &level_;
    ServerNetworkSystem *network_;
    NetworkIdentifier owner_;
    ClientBlobCache::Server::ActiveTransfersManager *client_cache_;
    SubClientId sub_client_id_;
    BlockPos last_chunk_update_position_;
    std::uint32_t last_chunk_update_radius_;
    std::uint32_t handle_for_chunk_build_order_updates_;
    int chunks_sent_since_start_;
    std::shared_ptr<ChunkViewSource> source_;
    std::shared_ptr<ChunkSource> network_chunk_source_;
    GridArea<std::shared_ptr<LevelChunk>>::AddCallback add_callback_;
    std::string cache_serialize_buffer_;
    std::unordered_map<ChunkPositionAndDimension, std::weak_ptr<LevelChunk>> queued_chunks_;
    std::chrono::steady_clock::time_point last_generation_request_queued_;
    bool waiting_for_server_chunks_;
    bool initial_spawn_done_;
    std::queue<ClientGenerationRequestHandler> generation_requests_;
};
BEDROCK_STATIC_ASSERT_SIZE(NetworkChunkPublisher, 488, 448);
