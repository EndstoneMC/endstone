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

#include "bedrock/bedrock.h"
#include "bedrock/world/container.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/level/block/actor/vanilla_block_actor.h"

class LecternBlockActor : public VanillaBlockActor, public Container {
public:
    static constexpr BlockActorType TypeId = BlockActorType::Lectern;

    // Endstone
    [[nodiscard]] int getPage() const { return page_; }
    void setPage(int page) { page_ = page; }
    [[nodiscard]] int getTotalPages() const { return total_pages_; }
    [[nodiscard]] const ItemStack &getBook() const { return book_; }

private:
    int page_;
    int total_pages_;
    ItemStack book_;
};
BEDROCK_STATIC_ASSERT_SIZE(LecternBlockActor, 808, 696);
