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

#include "bedrock/util/string_byte_output.h"
#include "bedrock/world/item/item_descriptor_count.h"
#include "bedrock/world/item/registry/item_registry_manager.h"

class NetworkItemStackDescriptor : public ItemDescriptorCount {
public:
    NetworkItemStackDescriptor(const ItemStack &item_stack)
        : ItemDescriptorCount(item_stack.getDescriptor(), item_stack.getCount())
    {
        include_net_ids_ = false;
        net_id_variant_ = item_stack.getItemStackNetIdVariant();
        if (auto *block = item_stack.getBlock()) {
            block_runtime_id_ = block->getRuntimeId();
        }
        if (!item_stack.isNull()) {
            if (const auto item = ItemRegistryManager::getItemRegistry().getItem(item_stack.getId()); !item.isNull()) {
                StringByteOutput stream(user_data_buffer_);
                item->writeUserData(item_stack, stream);
            }
        }
    }

private:
    bool include_net_ids_{false};
    ItemStackNetIdVariant net_id_variant_{};
    BlockRuntimeId block_runtime_id_{0};
    std::string user_data_buffer_;
};
