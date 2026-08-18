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

#include <memory>
#include <type_traits>

#include "bedrock/world/level/block/actor/vanilla_block_actor.h"
#include "endstone/block/container.h"
#include "endstone/core/block/block_state.h"
#include "endstone/core/inventory/inventory.h"

namespace endstone::core {

template <typename Interface = Container>
    requires std::is_base_of_v<Container, Interface>
class EndstoneContainerBase : public EndstoneBlockStateBase<Interface> {
public:
    EndstoneContainerBase(const EndstoneBlock &block, ::BlockActor &block_actor, bool use_snapshot)
        : EndstoneBlockStateBase<Interface>(block, block_actor, use_snapshot),
          inventory_(std::make_unique<EndstoneInventory>([this]() -> ::Container & { return getContainer(); }))
    {
    }

    [[nodiscard]] Inventory &getInventory() const override
    {
        return *inventory_;
    }

protected:
    [[nodiscard]] ::Container &getContainer() const
    {
        auto &block_actor = static_cast<::VanillaBlockActor &>(this->template getBlockActor<::BlockActor>());
        auto *container = block_actor.getContainer();
        if (container == nullptr) {
            throw std::runtime_error("Trying to access a container block state that is no longer valid.");
        }
        return *container;
    }

private:
    std::unique_ptr<EndstoneInventory> inventory_;
};

using EndstoneContainer = EndstoneContainerBase<Container>;

}  // namespace endstone::core
