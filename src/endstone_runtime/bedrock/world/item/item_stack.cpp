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

#include "bedrock/world/item/item_stack.h"
#include "endstone/detail/hook.h"

// TODO(daoge_cmd): Modify ENDSTONE_FACTORY_IMPLEMENT macros to support multiple factory functions
// std::unique_ptr<ItemStack> ItemStack::create(std::string_view name, int count, int aux_value, CompoundTag const* user_data)
// {
//     ENDSTONE_FACTORY_IMPLEMENT(ItemStack, name, count, aux_value, user_data);
// }

std::unique_ptr<ItemStack> ItemStack::create(Item const& item, int count, int aux_value, CompoundTag const* user_data)
{
    ENDSTONE_FACTORY_IMPLEMENT(ItemStack, item, count, aux_value, user_data);
}