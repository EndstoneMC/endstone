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

#include <cstdint>

#include "bedrock/common_types.h"
#include "bedrock/world/level/biome/components/biome_component_storage.h"
#include "bedrock/world/level/biome/mob_spawner_data.h"
#include "bedrock/world/level/block_pos.h"

enum class OceanTempCategory {
    COLD = 0,
    WARM = 1,
};

struct OceanRuinConfiguration {
    OceanTempCategory type;
    float large_probability;
    float cluster_probability;
};

class BlockSource;
class Biome {
public:
    Biome(BiomeIdType, std::string_view);

    virtual ~Biome() = 0;

    [[nodiscard]] const std::string &getFullName() const
    {
        return hash.getString();
    }

    [[nodiscard]] const std::string getNameWithoutNamespace() const;

    [[nodiscard]] BiomeIdType getId() const
    {
        return id_;
    }

    float temperature;
    float downfall;
    float red_spore_density;
    float blue_spore_density;
    float ash_density;
    float white_ash_density;
    float snow_accumulation;
    float foliage_snow;
    float min_snow_level;
    float max_snow_level;
    float depth;
    float scale;
    int map_water_color;
    const float water_transparency;
    bool rain;
    OceanRuinConfiguration ocean_ruin_config;

protected:
    MobList mobs_;
    MobCategoryMap mobs_map_;
    struct CachedClientComponentData {
        bool has_noise_based_color_palette;
        bool is_roofed_forest;
    };
    static_assert(sizeof(CachedClientComponentData) == 2);
    CachedClientComponentData cached_client_component_data_;

private:
    BiomeIdType id_;
    BiomeComponentStorage biome_component_storage_;
    const HashedString hash;
};
