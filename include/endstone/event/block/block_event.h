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

#include "endstone/block/block.h"
#include "endstone/event/event.h"

namespace endstone {

/**
 * @brief Represents an Block-related event
 */
class BlockEvent : public Event {
public:
    explicit BlockEvent(std::unique_ptr<Block> block) : block_(std::move(block)){};
    ~BlockEvent() override = default;

    /**
     * @brief Gets the block involved in this event.
     *
     * @return The Block which block is involved in this event
     */
    [[nodiscard]] Block &getBlock() const
    {
        return *block_;
    }

private:
    std::unique_ptr<Block> block_;
};

}  // namespace endstone
