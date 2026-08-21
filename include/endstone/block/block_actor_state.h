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

#include "endstone/block/block_state.h"

namespace endstone {

/**
 * Represents a captured state of a block entity.
 */
class BlockActorState : public BlockState {
public:
    ~BlockActorState() override = default;

    /**
     * Gets whether this state is backed by an independent block entity snapshot.
     *
     * @return `true` if this state is a snapshot, otherwise `false`.
     */
    [[nodiscard]] virtual bool isSnapshot() const = 0;
};

}  // namespace endstone
