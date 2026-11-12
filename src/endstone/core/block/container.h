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
#include <optional>
#include <string>
#include <type_traits>

#include "bedrock/safety/redactable_string.h"
#include "bedrock/world/level/block/actor/vanilla_block_actor.h"
#include "endstone/block/container.h"
#include "endstone/core/block/block_state.h"
#include "endstone/core/inventory/inventory.h"

namespace endstone::core {

template <typename Interface = Container>
    requires std::is_base_of_v<BlockActorState, Interface>
class EndstoneContainerBase : public EndstoneBlockStateBase<Interface> {
public:
    EndstoneContainerBase(const EndstoneBlock &block, ::BlockActor &block_actor, bool use_snapshot)
        : EndstoneBlockStateBase<Interface>(block, block_actor, use_snapshot),
          inventory_(std::make_unique<EndstoneInventory>([this]() -> ::Container & { return getLiveContainer(); })),
          snapshot_inventory_(std::make_unique<EndstoneInventory>([this]() -> ::Container & { return getContainer(); }))
    {
    }

    EndstoneContainerBase(const EndstoneBlock &block, ::BlockActor &block_actor, bool use_snapshot,
                          std::unique_ptr<Inventory> inventory, std::unique_ptr<Inventory> snapshot_inventory)
        : EndstoneBlockStateBase<Interface>(block, block_actor, use_snapshot), inventory_(std::move(inventory)),
          snapshot_inventory_(std::move(snapshot_inventory))
    {
    }

    [[nodiscard]] Inventory &getInventory() const override { return *inventory_; }

    [[nodiscard]] Inventory &getSnapshotInventory() const override { return *snapshot_inventory_; }

    [[nodiscard]] std::optional<std::string> getCustomName() const
    {
        auto &block_actor = getVanillaBlockActor();
        if (!block_actor.hasCustomName()) {
            return std::nullopt;
        }
        return block_actor.getCustomName().getUnredactedString();
    }

    void setCustomName(std::optional<std::string> name)
    {
        getVanillaBlockActor().setCustomName(name.value_or(std::string{}));
    }

protected:
    bool applyTo(::ILevel &level, ::BlockActor &block_actor) const override
    {
        if (!EndstoneBlockStateBase<Interface>::applyTo(level, block_actor)) {
            return false;
        }
        if constexpr (std::is_base_of_v<Container, Interface>) {
            if (this->isSnapshot() && block_actor.getType() != ::BlockActorType::DataDriven &&
                !getVanillaBlockActor().hasCustomName()) {
                static_cast<::VanillaBlockActor &>(block_actor).setCustomName({});
            }
        }
        return true;
    }

    [[nodiscard]] ::VanillaBlockActor &getVanillaBlockActor() const
    {
        auto &block_actor = this->template getBlockActor<::BlockActor>();
        if (block_actor.getType() == ::BlockActorType::DataDriven) {
            throw std::runtime_error("Trying to access a container block state that is no longer valid.");
        }
        return static_cast<::VanillaBlockActor &>(block_actor);
    }

    [[nodiscard]] ::Container &getContainer() const
    {
        return getContainer(this->template getBlockActor<::BlockActor>());
    }

    [[nodiscard]] ::Container &getLiveContainer() const
    {
        return getContainer(this->template getLiveBlockActor<::BlockActor>());
    }

private:
    [[nodiscard]] static ::Container &getContainer(::BlockActor &block_actor)
    {
        ::Container *container = nullptr;
        if (block_actor.getType() != ::BlockActorType::DataDriven) {
            container = static_cast<::VanillaBlockActor &>(block_actor).getContainer();
        }
        if (container == nullptr) {
            throw std::runtime_error("Trying to access a container block state that is no longer valid.");
        }
        return *container;
    }

    std::unique_ptr<Inventory> inventory_;
    std::unique_ptr<Inventory> snapshot_inventory_;
};

using EndstoneContainer = EndstoneContainerBase<Container>;

}  // namespace endstone::core
