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

#include "endstone/core/block/item_frame.h"

#include <cmath>

#include "endstone/check.h"
#include "endstone/core/inventory/item_stack.h"

namespace endstone::core {

EndstoneItemFrame::EndstoneItemFrame(const EndstoneBlock &block, ::ItemFrameBlockActor &item_frame, bool use_snapshot)
    : EndstoneBlockStateBase<ItemFrame>(block, item_frame, use_snapshot)
{
}

std::optional<ItemStack> EndstoneItemFrame::getItem() const
{
    const auto &item = getItemFrame().getFramedItem();
    if (item.isNull()) {
        return std::nullopt;
    }
    return EndstoneItemStack::fromMinecraft(item);
}

void EndstoneItemFrame::setItem(const std::optional<ItemStack> &item)
{
    if (!item.has_value()) {
        getItemFrame().setItem(getBlockSource(), ItemInstance::EMPTY_ITEM, nullptr);
        return;
    }
    const auto minecraft_item = EndstoneItemStack::toMinecraft(item.value());
    getItemFrame().setItem(getBlockSource(), ItemInstance(minecraft_item), nullptr);
}

float EndstoneItemFrame::getItemDropChance() const
{
    return getItemFrame().getDropChance();
}

void EndstoneItemFrame::setItemDropChance(float chance)
{
    Preconditions::checkArgument(chance >= 0.0F && chance <= 1.0F, "Item drop chance must be between 0.0 and 1.0.");
    getItemFrame().setDropChance(chance);
    getItemFrame().setChanged();
}

Rotation EndstoneItemFrame::getRotation() const
{
    // BDS keeps the rotation in [0, 360) degrees, in steps of ROTATION_DEGREES
    const auto steps = std::lround(getItemFrame().getRotation() / ::ItemFrameBlockActor::ROTATION_DEGREES);
    return static_cast<Rotation>(steps % ::ItemFrameBlockActor::NUM_ROTATIONS);
}

void EndstoneItemFrame::setRotation(Rotation rotation)
{
    // ItemFrameBlock::use rotates by one step the same way: write the degrees back, then setChanged
    getItemFrame().setRotation(static_cast<float>(rotation) * ::ItemFrameBlockActor::ROTATION_DEGREES);
    getItemFrame().setChanged();
}

}  // namespace endstone::core
