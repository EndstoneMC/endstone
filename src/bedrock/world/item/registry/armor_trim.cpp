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

#include "bedrock/world/item/registry/armor_trim.h"

#include "bedrock/nbt/compound_tag.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_stack_base.h"

ArmorTrim::ArmorTrim(HashedString pattern_id, HashedString material_id)
    : pattern_id_(std::move(pattern_id)), material_id_(std::move(material_id))
{
}

void ArmorTrim::setTrim(ItemStackBase &instance, ArmorTrim trim)
{
    const auto *item = instance.getItem();
    if (item == nullptr || !item->isHumanoidArmor() || !item->isTrimAllowed()) {
        return;
    }
    if (instance.getUserData() == nullptr) {
        instance.setUserData(std::make_unique<CompoundTag>());
    }
    auto *user_data = instance.getUserData();
    if (user_data == nullptr) {
        return;
    }
    CompoundTag trim_tag;
    trim_tag.putString("Material", trim.material_id_.getString());
    trim_tag.putString("Pattern", trim.pattern_id_.getString());
    user_data->putCompound("Trim", std::move(trim_tag));
}
