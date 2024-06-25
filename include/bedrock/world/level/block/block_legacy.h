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

#include "bedrock/forward.h"
#include "bedrock/world/level/block/block_component_storage.h"

using Brightness = std::uint8_t;
using NewBlockID = std::uint16_t;

class BlockLegacy : public BlockComponentStorage {
    struct NameInfo {
        HashedString raw_name;             // +0
        std::string namespace_name;        // +48
        HashedString full_name;            // +80
        HashedString pre_flattening_name;  // +128
    };

public:
private:
    std::string description_id_;                    // +40
    NameInfo name_info_;                            // +72
    BlockProperty properties_;                      // +248 (+208)
    bool fancy_;                                    // +256
    BlockRenderLayer render_layer_;                 // +260 (+220)
    bool render_layer_can_render_as_opaque_;        // +264
    BlockActorType block_entity_type_;              // +268 (+228)
    bool animated_texture_;                         // +272
    float brightness_gamma_;                        // +276
    float thickness_;                               // +280 (+240)
    bool can_slide_;                                // +284 (+244)
    bool can_react_to_neighbors_during_instatick_;  // +285 (+245)
    bool is_interaction_;                           // +286 (+246)
    float gravity_;                                 // +288 (+248)
    Material *material_;                            // +296 (+256)
    bool heavy_;                                    // +304 (+264)
    float particle_quantity_scalar_;                // +308 (+268)
    CreativeItemCategory creative_item_category_;   // +312 (+272)
    std::string creative_group_;                    // +320
    bool is_hidden_in_commands_;                    // +352 (+304)
    bool allow_runes_;                              // +353 (+305)
    bool can_be_broken_from_falling_;               // +354 (+306)
    bool can_be_original_surface_;                  // +355 (+307)
    bool solid_;                                    // +356 (+308)
    bool pushes_out_items_;                         // +357 (+309)
    bool ignore_block_for_inside_cube_renderer_;    // +358 (+310)
    bool is_trapdoor_;                              // +359
    bool is_door_;                                  // +360
    bool is_opaque_full_block_;                     // +361 (+313)
    float translucency_;                            // +364 (+316)
    bool should_random_tick_;                       // +368 (+320)
    bool should_random_tick_extra_layer_;           // +369 (+321)
    bool is_mob_piece_;                             // +370
    bool can_be_extra_block_;                       // +371 (+323)
    bool can_propagate_brightness_;                 // +372 (+324)
    Brightness light_block_;                        // +373 (+325)
    Brightness light_emission_;                     // +374 (+326)
    FlameOdds flame_odds_;                          // +376 (+328)
    BurnOdds burn_odds_;                            // +380 (+332)
    LavaFlammable lava_flammable_;                  // +384 (+336)
    float destroy_speed_;                           // +388 (+340)
    float explosion_resistence_;                    // +392 (+344)
    mce::Color map_color_;                          // +396 (+348)
    float friction_;                                // +412 (+364)
    BlockTintType block_tint_type_;                 // +416 (+368)
    bool unknown_;                                  // +420
    NewBlockID id_;                                 // +422 (+374)
    // ...
};
