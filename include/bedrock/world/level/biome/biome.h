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

#include "bedrock/core/math/color.h"
#include "bedrock/forward.h"
#include "bedrock/gamerefs/weak_ref.h"

class Biome {
    enum class OceanTempCategory : std::int32_t {
        COLD = 0,
        WARM = 1,
    };

    struct CachedClientComponentData {
        bool has_noise_based_color_palette;
        bool is_roofed_forest;
    };

    struct OceanRuinConfiguration {
        OceanTempCategory type;
        float large_probability;
        float cluster_probability;
    };

public:
    virtual ~Biome() = 0;

    [[nodiscard]] const std::string &getName() const
    {
        return hash_.getString();
    }

    [[nodiscard]] int getId() const
    {
        return id_;
    }

private:
    HashedString hash_;                                       // +0
    float temperature_;                                       // +56
    float downfall_;                                          // +60
    float red_spore_density_;                                 // +64
    float blue_spore_density_;                                // +68
    float ash_density_;                                       // +72
    float white_ash_density_;                                 // +76
    float snow_accumulation_;                                 // +80
    float foliage_snow_;                                      // +84
    float min_snow_level_;                                    // +88
    float max_snow_level_;                                    // +92
    float depth_;                                             // +96
    float scale_;                                             // +100
    mce::Color water_color_;                                  // +104
    float water_transparency_;                                // +120
    bool rain_;                                               // +124
    WeakRef<const FogDefinition> fog_definition_;             // +128
    OceanRuinConfiguration ocean_ruin_config_;                // +144
    std::vector<std::shared_ptr<void *>> mobs_;               // +160
    CachedClientComponentData cached_client_component_data_;  // +184
    const int id_;                                            // +188
    // BiomeComponentStorage biome_component_storage_;
};
