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
#include "endstone/command/command_sender.h"

namespace endstone {

/**
 * @brief Represents a block command sender
 */
class BlockCommandSender : public virtual CommandSender {
public:
    /**
     * @brief Returns the block this command sender belongs to
     *
     * @return Block for the command sender
     */
    [[nodiscard]] virtual std::unique_ptr<Block> getBlock() const = 0;
};

}  // namespace endstone
