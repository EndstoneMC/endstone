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
#include <vector>

#include "bedrock/core/file/path.h"
#include "bedrock/core/threading/async.h"
#include "bedrock/forward.h"
#include "bedrock/nbt/compound_tag.h"
#include "bedrock/world/level/chunk/chunk_source.h"
#include "bedrock/world/level/storage/db_storage_performance_data.h"

struct PlayerStorageIds {
    std::string msa_id;
    std::string guest_id;
    std::string platform_id;
    std::string platform_online_id;
    std::string platform_offline_id;
    std::string self_signed_id;
    std::string random_client_id;
};

class LevelStorage {
public:
    enum class StatsType;

    virtual ~LevelStorage() = 0;
};
