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

#include <atomic>
#include <functional>
#include <unordered_map>

#include <gsl/gsl>

#include "bedrock/core/string/string_hash.h"
#include "bedrock/core/utility/enable_non_owner_references.h"
#include "bedrock/core/utility/pub_sub/subscription.h"
#include "bedrock/platform/brstd/flat_set.h"
#include "bedrock/util/tag_registry.h"
#include "bedrock/world/level/biome/biome.h"
#include "bedrock/world/level/biome/registry/well_known_biome_tags.h"

class BiomeRegistry : public Bedrock::EnableNonOwnerReferences {
public:
    void forEachBiome(std::function<void(Biome const &)>) const;
    void forEachNonConstBiome(std::function<void(Biome &)>);

private:
    using BiomeNameLookupMap = std::unordered_map<HashType64, std::unique_ptr<Biome>>;
    struct BiomeComparator {};

    WellKnownBiomeTags well_known_biome_tags_;
    bool surface_builders_resolved;
    bool client_initialized_;
    BiomeNameLookupMap biomes_by_name_;
    brstd::flat_set<gsl::not_null<Biome *>, BiomeComparator> biomes_by_id_;  // +1080
    std::vector<std::pair<const BiomeIdType, const std::string &>> removed_biomes_id_and_full_name_;
    std::uint32_t next_custom_biome_id_;
    std::atomic<bool> closed_for_registration_;
    bool load_from_all_packs_;
    BiomeTagRegistry tag_registry_;
    Biome *empty_biome_;  // +1256
    Bedrock::PubSub::Subscription on_save_subscription_;
    Bedrock::PubSub::Subscription on_level_storage_manager_start_leave_game_subscription_;
};
