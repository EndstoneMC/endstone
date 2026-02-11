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
    struct SeasonTextureRowSettings {
        float temperature;
        float downfall;
        float foliage_snow;
    };

private:
    using BiomeNameLookupMap = std::unordered_map<HashType64, std::unique_ptr<Biome>>;
    struct BiomeComparator {};

    BiomeTagRegistry tag_registry_;

public:
    const WellKnownBiomeTags well_known_biome_tags;
    bool surface_builders_resolved;
    bool client_initialized;
    BiomeNameLookupMap biomes_by_name;
    brstd::flat_set<gsl::not_null<Biome *>, BiomeComparator> biomes_by_id;  // +1080
    std::vector<std::pair<const BiomeIdType, const std::string &>> removed_biomes_id_and_full_name;
    std::uint32_t next_custom_biome_id;
    std::atomic<bool> closed_for_registration;
    Biome *empty_biome;  // +1256
    Bedrock::PubSub::Subscription on_save_subscription;
    Bedrock::PubSub::Subscription on_level_storage_manager_start_leave_game_subscription;
    std::vector<SeasonTextureRowSettings> season_texture_row_settings;
};
