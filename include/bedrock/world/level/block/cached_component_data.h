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

struct CachedComponentData {
    Brightness emissive_brightness;     // +0
    bool is_solid;                      // +1
    BlockOcclusionType occlusion_type;  // +4
    char pad[48];                       // +8
    bool is_full_block_geometry;        // +56
    bool use_new_tessellation;          // +57
    bool needs_legacy_top_rotation;     // +58
    bool is_opaque_full_block;          // +59
    Brightness light_emission;          // +60
    Brightness light;                   // +61
    float explosion_resistance;         // +64
    BurnOdds burn_odds;                 // +68
    FlameOdds flame_odds;               // +72
    float friction;                     // +76
    float destroy_speed;                // +80
};
