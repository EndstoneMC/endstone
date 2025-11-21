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

#include "endstone/event/block/block_event.h"
#include "endstone/event/cancellable.h"

namespace endstone {

class BlockGrowEvent : public Cancellable<BlockEvent> {
public:
    explicit BlockGrowEvent(std::unique_ptr<Block> block, std::unique_ptr<BlockState> new_state)
        : Cancellable(std::move(block)), new_state_(std::move(new_state))
    {
    }
    ~BlockGrowEvent() override = default;

    inline static const std::string NAME = "BlockGrowEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    [[nodiscard]] BlockState &getToBlock() const
    {
        return *new_state_;
    }

private:
    std::unique_ptr<BlockState> new_state_;
};

}  // namespace endstone
