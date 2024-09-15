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

struct BlockComponentDirectData {
    class BlockTransformationComponent const *block_transformation_component;          // +0
    class BlockCollisionBoxComponent const *block_collision_box_component;             // +8
    class BlockSelectionBoxComponent const *block_selection_box_component;             // +16
    class BlockRedstoneComponent const *block_redstone_component;                      // +24
    class BlockGeometryComponent const *block_geometry_component;                      // +32
    class BlockBakedMaterialDataComponent const *block_baked_material_data_component;  // +40
    bool is_full_block_geometry;                                                       // +48
    bool use_new_tessellation;                                                         // +49
    bool needs_legacy_top_rotation;                                                    // +50
    bool is_opaque_full_block;                                                         // +51
    Brightness light_emission;                                                         // +52
    Brightness light;                                                                  // +53
    float explosion_resistance;                                                        // +56
    BurnOdds burn_odds;                                                                // +60
    FlameOdds flame_odds;                                                              // +64
    float friction;                                                                    // +68
    float destroy_speed;                                                               // +72
};
