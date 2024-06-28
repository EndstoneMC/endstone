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

#include <string>

#include "bedrock/common/game_version.h"
#include "bedrock/world/item/item_tag.h"
#include "bedrock/world/level/block/block_legacy.h"

class Item {
public:
    virtual ~Item() = 0;

    [[nodiscard]] std::int8_t getMaxStackSize() const
    {
        return max_stack_size_;
    }

    [[nodiscard]] std::int16_t getId() const
    {
        return id_;
    }

    [[nodiscard]] std::int16_t getMaxDamage() const
    {
        return max_damage_;
    }

private:
    std::string texture_atlas_file_;                  // +8
    int frame_count_;                                 // +40
    std::uint16_t use_anim_;                          // +44
    bool animates_in_toolbar_;                        // +46
    std::string hover_text_color_format_;             // +48
    std::uint64_t unknown_;                           // +80
    int atlas_total_frame_;                           // +88
    std::string icon_name_;                           // +96
    std::string atlas_name_;                          // +128
    std::int8_t max_stack_size_;                      // +160
    std::int16_t id_;                                 // +162
    std::string description_id_;                      // +168
    HashedString raw_name_id_;                        // +200
    std::string namespace_;                           // +248
    HashedString full_name_;                          // +280
    std::int16_t max_damage_;                         // +328
    std::int16_t unknown2_;                           // +330
    int max_use_duration_;                            // +332
    BaseGameVersion min_required_base_game_version_;  // +336
    WeakPtr<BlockLegacy> legacy_block_;               // +456
    CreativeItemCategory creative_category_;          // +464
    Item *crafting_remaining_item_;                   // +472
    std::string creative_group_;                      // +480
    std::uint64_t unknown3_;                          // +512
    int unknown4_;                                    // +520
    int unknown5_;                                    // +524
    std::vector<void *> unknown6_;                    // +528
    std::vector<void *> unknown7_;                    // +552
    std::vector<ItemTag> tags_;                       // +576
};
