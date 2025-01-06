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

#include "bedrock/core/string/string_hash.h"
#include "bedrock/core/utility/enable_non_owner_references.h"
#include "bedrock/util/tag_registry.h"
#include "bedrock/world/level/biome/biome.h"
#include "bedrock/world/level/biome/registry/well_known_biome_tags.h"

class BiomeRegistry : public Bedrock::EnableNonOwnerReferences {
public:
    void forEachBiome(std::function<void(Biome const &)>) const;
    void forEachNonConstBiome(std::function<void(Biome &)>);

private:
    using BiomeNameLookupMap = std::unordered_map<HashType64, std::unique_ptr<Biome>>;

    WellKnownBiomeTags well_known_biome_tags_;   // +24
    bool client_initialized_;                    // +1008
    BiomeNameLookupMap biomes_by_name_;          // +1016
    std::vector<Biome *> biomes_by_id_;          // +1080
    std::uint32_t next_id_;                      // +1104
    std::atomic<bool> closed_for_registration_;  // +1108
    bool load_from_packs_;                       // +1109
    BiomeTagRegistry tag_registry_;              // +1112
    Biome *empty_biome_;                         // +1240
};
