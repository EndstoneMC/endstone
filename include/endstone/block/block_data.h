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

namespace endstone {

class BlockData {
public:
    virtual ~BlockData() = default;

    /**
     * @brief Get the block type represented by this block data.
     *
     * @return the block type
     */
    [[nodiscard]] virtual std::string getType() const = 0;

    /**
     * @brief Gets the block states as a string, which when passed into a method such as
     * Server::createBlockData(type, block_states) will unambiguously recreate this instance.
     *
     * @return serialized data string for this block
     */
    [[nodiscard]] virtual std::string getBlockStates() const = 0;
};

}  // namespace endstone
