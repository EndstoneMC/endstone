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

#include "bedrock/world/level/saveddata/maps/map_item_tracked_actor.h"

void MapItemTrackedActor::setPixelDirty(std::uint32_t x, std::uint32_t y)
{
    if (needs_resend_) {
        min_dirty_x_ = std::min(min_dirty_x_, x);
        min_dirty_y_ = std::min(min_dirty_y_, y);
        max_dirty_x_ = std::max(max_dirty_x_, x);
        max_dirty_y_ = std::max(max_dirty_y_, y);
    }
    else {
        needs_resend_ = true;
        min_dirty_x_ = x;
        min_dirty_y_ = y;
        max_dirty_x_ = x;
        max_dirty_y_ = y;
    }
}
