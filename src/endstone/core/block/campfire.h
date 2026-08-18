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

#include <optional>

#include "bedrock/world/level/block/actor/campfire_block_actor.h"
#include "endstone/block/campfire.h"
#include "endstone/core/block/block_state.h"

namespace endstone::core {

class EndstoneCampfire : public EndstoneBlockStateBase<Campfire> {
public:
    EndstoneCampfire(const EndstoneBlock &block, ::CampfireBlockActor &campfire, bool use_snapshot);

    [[nodiscard]] int getSize() const override;
    [[nodiscard]] std::optional<ItemStack> getItem(int index) const override;
    void setItem(int index, const std::optional<ItemStack> &item) override;
    [[nodiscard]] int getCookTime(int index) const override;
    void setCookTime(int index, int cook_time) override;

private:
    void checkIndex(int index) const;

    ::CampfireBlockActor &campfire_;
};

}  // namespace endstone::core
