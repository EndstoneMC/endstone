

# File player\_bucket\_event.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**event**](dir_f1d783c0ad83ee143d16e768ebca51c8.md) **>** [**player**](dir_7c05c37b25e9c9eccd9c63c2d313ba28.md) **>** [**player\_bucket\_event.h**](player__bucket__event_8h.md)

[Go to the documentation of this file](player__bucket__event_8h.md)


```C++
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
#include <utility>

#include "endstone/block/block_face.h"
#include "endstone/event/cancellable.h"
#include "endstone/event/player/player_event.h"
#include "endstone/inventory/equipment_slot.h"
#include "endstone/inventory/item_stack.h"
#include "endstone/util/pointers.h"

namespace endstone {

class Block;
class ItemType;

class PlayerBucketEvent : public Cancellable<PlayerEvent> {
public:
    PlayerBucketEvent(const NotNull<Player> &player, const Nullable<Block> &block, const NotNull<Block> &block_clicked,
                      BlockFace block_face, const ItemType &bucket, std::optional<ItemStack> item_stack,
                      EquipmentSlot hand)
        : Cancellable(player), block_(block), block_clicked_(block_clicked), block_face_(block_face), bucket_(bucket),
          item_stack_(std::move(item_stack)), hand_(hand)
    {
    }
    ~PlayerBucketEvent() override = default;

    [[nodiscard]] const Nullable<Block> &getBlock() const { return block_; }

    [[nodiscard]] const NotNull<Block> &getBlockClicked() const { return block_clicked_; }

    [[nodiscard]] BlockFace getBlockFace() const { return block_face_; }

    [[nodiscard]] const ItemType &getBucket() const { return bucket_; }

    [[nodiscard]] EquipmentSlot getHand() const { return hand_; }

    [[nodiscard]] const std::optional<ItemStack> &getItemStack() const { return item_stack_; }

    void setItemStack(std::optional<ItemStack> item_stack) { item_stack_ = std::move(item_stack); }

private:
    Nullable<Block> block_;
    NotNull<Block> block_clicked_;
    BlockFace block_face_;
    const ItemType &bucket_;
    std::optional<ItemStack> item_stack_;
    EquipmentSlot hand_;
};

}  // namespace endstone
```


