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

#include "bedrock/core/container/dense_enum_map.h"
#include "bedrock/shared_types/legacy/facing.h"
#include "bedrock/voxel_shape/registry_handle.h"
#include "bedrock/world/level/block/detection_rule.h"
#include "bedrock/world/level/block/flammable.h"

struct BlockComponentDirectData {
    enum LayerBitMask : std::uint32_t {
        NONE = 0x0,
        INIT = 0x1,
        RENDERING = 0x2,
    };

    using FaceShapeHandles =
        Bedrock::DenseEnumMap<SharedTypes::Facing, VoxelShapes::RegistryHandle, SharedTypes::Facing::MAX>;
    class BlockTransformationComponent const *block_transformation_component;
    class BlockCollisionBoxComponent const *block_collision_box_component;
    class BlockSelectionBoxComponent const *block_selection_box_component;
    class BlockRedstoneComponent const *block_redstone_component;
    class BlockGeometryComponent const *block_geometry_component;
    // class BlockBakedMaterialDataComponent const *block_baked_material_data_component;
    // class BlockItemVisualComponent const *block_item_visual_component;
    bool is_full_block_geometry;
    bool use_new_tessellation;
    bool needs_legacy_top_rotation;
    bool is_opaque_full_block;
    Brightness light_emission;
    Brightness light;
    LavaFlammable lava_flammable;
    FlameOdds flame_odds;
    BurnOdds burn_odds;
    float explosion_resistance;
    float friction;
    float destroy_speed;
    DetectionRule water_detection_rule;
    VoxelShapes::RegistryHandle occlusion_shape_handle;
    FaceShapeHandles occlusion_shape_handles_by_face;
    LayerBitMask finalized;
    class BlockDestructibleByMiningComponent const *destructible_by_mining_component;
};
