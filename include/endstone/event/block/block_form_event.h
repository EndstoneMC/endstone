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

#include "endstone/event/block/block_grow_event.h"

namespace endstone {
/**
 * @brief Called when a block is formed or spreads based on world conditions.
 *
 * Use BlockSpreadEvent to catch blocks that actually spread and don't just "randomly" form.
 *
 * Examples:
 * - Snow forming due to a snow storm.
 * - Ice forming in a snowy Biome like Taiga or Tundra.
 * - Obsidian / Cobblestone forming due to contact with water.
 * - Concrete forming due to mixing of concrete powder and water.
 *
 * If a Block Form event is cancelled, the block will not be formed.
 */
class BlockFormEvent final : public BlockGrowEvent {
public:
    ENDSTONE_EVENT(BlockFormEvent);
    using BlockGrowEvent::BlockGrowEvent;
};

}  // namespace endstone
