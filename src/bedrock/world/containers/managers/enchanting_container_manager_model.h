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

#include <functional>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/world/containers/managers/container_manager_model.h"
#include "bedrock/world/item/enchanting/item_enchant_option.h"
#include "bedrock/world/level/block_pos.h"

class EnchantingContainerManagerModel : public ContainerManagerModel {
public:
    EnchantingContainerManagerModel(ContainerID container_id, Player &player, const BlockPos &block_pos);
    ~EnchantingContainerManagerModel() override = 0;

    [[nodiscard]] const BlockPos &getBlockPos() const { return block_pos_; }
    [[nodiscard]] const std::vector<ItemEnchantOption> &getEnchantOptions() const { return enchant_options_; }
    [[nodiscard]] std::vector<ItemEnchantOption> &getEnchantOptions() { return enchant_options_; }

private:
    BlockPos block_pos_;
    bool should_book_be_open_;
    std::vector<ItemEnchantOption> enchant_options_;
    std::function<void(EnchantingContainerManagerModel &)> options_changed_callback_;
};
BEDROCK_STATIC_ASSERT_SIZE(EnchantingContainerManagerModel, 496, 400);
