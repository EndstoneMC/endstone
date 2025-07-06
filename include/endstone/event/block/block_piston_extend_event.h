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

#include "endstone/event/block/block_piston_event.h"

namespace endstone {

/**
 * @brief Called when a piston extends
 */
class BlockPistonExtendEvent final : public BlockPistonEvent {
public:
    ENDSTONE_EVENT(BlockPistonExtendEvent);
    using BlockPistonEvent::BlockPistonEvent;
};

}  // namespace endstone
