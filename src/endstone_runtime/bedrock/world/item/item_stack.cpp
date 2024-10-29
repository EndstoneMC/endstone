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

#include <bedrock/world/item/registry/item_registry_manager.h>

#include "endstone/detail/hook.h"

std::unique_ptr<ItemStack> ItemStack::create()
{
    std::unique_ptr<ItemStack> (*fp)() = &ItemStack::create;
    ENDSTONE_FACTORY_IMPLEMENT_OVERLOAD(ItemStack, fp);
}

std::unique_ptr<ItemStack> ItemStack::create(const ItemStack &rhs)
{
    std::unique_ptr<ItemStack> (*fp)(const ItemStack &) = &ItemStack::create;
    ENDSTONE_FACTORY_IMPLEMENT_OVERLOAD(ItemStack, fp, rhs);
}

std::unique_ptr<ItemStack> ItemStack::create(Item const &item, int count, int aux_value, CompoundTag const *user_data)
{
    std::unique_ptr<ItemStack> (*fp)(Item const &, int, int, CompoundTag const *) = &ItemStack::create;
    ENDSTONE_FACTORY_IMPLEMENT_OVERLOAD(ItemStack, fp, item, count, aux_value, user_data);
}

std::unique_ptr<ItemStack> ItemStack::create(std::string_view name, int count, int aux_value,
                                             CompoundTag const *user_data)
{
    const auto item = ItemRegistryManager::getItemRegistry().getItem(std::string(name));
    if (item.isNull()) {
        return create();
    }
    return create(*item, count, aux_value, user_data);
}
