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

#include "bedrock/scripting/event_handlers/script_item_gameplay_handler.h"

#include <optional>
#include <string_view>
#include <type_traits>
#include <utility>

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/item/registry/item_registry_manager.h"
#include "bedrock/world/level/block/block.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/game_type.h"
#include "endstone/core/block/block.h"
#include "endstone/core/block/block_face.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_bucket_fill_event.h"
#include "endstone/event/player/player_interact_event.h"
#include "endstone/runtime/bedrock_hooks/actor_interaction.h"
#include "endstone/runtime/bedrock_hooks/bucket_fill.h"
#include "endstone/runtime/vtable_hook.h"

namespace {

std::optional<std::string_view> getFilledBucketName(const ::Block &block)
{
    const auto &name = block.getName().getString();
    static const HashedString liquid_depth{"liquid_depth"};
    if (name == "minecraft:water") {
        if (block.getState<int>(liquid_depth) != 0) {
            return std::nullopt;
        }
        return "minecraft:water_bucket";
    }
    if (name == "minecraft:lava") {
        if (block.getState<int>(liquid_depth) != 0) {
            return std::nullopt;
        }
        return "minecraft:lava_bucket";
    }
    if (name == "minecraft:powder_snow") {
        return "minecraft:powder_snow_bucket";
    }

    if (name == "minecraft:cauldron") {
        static const HashedString fill_level{"fill_level"};
        if (block.getState<int>(fill_level) != 6) {
            return std::nullopt;
        }

        static const HashedString cauldron_liquid{"cauldron_liquid"};
        switch (block.getState<int>(cauldron_liquid)) {
        case 0:
            return "minecraft:water_bucket";
        case 1:
            return "minecraft:lava_bucket";
        case 2:
            return "minecraft:powder_snow_bucket";
        default:
            return std::nullopt;
        }
    }
    return std::nullopt;
}

struct PendingBucketFill {
    ::Player *player;
    BlockPos position;
    std::optional<endstone::ItemStack> item_stack;
    bool write_item_stack;
};

thread_local std::optional<PendingBucketFill> pending_bucket_fill;

InteractionResult bucketUseOn(
    [[maybe_unused]] const ::Item *item,
    ::ItemStack &item_stack,
    ::Actor &actor,
    BlockPos position,
    [[maybe_unused]] FacingID face,
    [[maybe_unused]] const Vec3 &click_pos)
{
    const auto result = ENDSTONE_VHOOK_CALL_ORIGINAL(&bucketUseOn, item, item_stack, actor, position, face, click_pos);
    if (!pending_bucket_fill || !actor.isPlayer()) {
        return result;
    }

    auto *player = static_cast<::Player *>(&actor);
    if (pending_bucket_fill->player != player || pending_bucket_fill->position != position) {
        pending_bucket_fill.reset();
        return result;
    }

    if (result.isSuccessful() && pending_bucket_fill->write_item_stack) {
        item_stack.setUserData(nullptr);
        item_stack = pending_bucket_fill->item_stack
                         ? endstone::core::EndstoneItemStack::toMinecraft(*pending_bucket_fill->item_stack)
                         : ::ItemStack::EMPTY_ITEM;
    }
    pending_bucket_fill.reset();
    return result;
}

}  // namespace

void endstone::runtime::installBucketFillHook()
{
    static bool installed = false;
    if (installed) {
        return;
    }

    const auto registry = ItemRegistryManager::getItemRegistry();
    if (!registry.isValid()) {
        return;
    }

    int aux_value = 0;
    const auto bucket = registry.lookupByName(aux_value, "minecraft:bucket");
    if (bucket.isNull()) {
        return;
    }

    // Item::_useOn is the last virtual declared by the version-matched Item ABI.
    endstone::runtime::vhook::create<129>(bucket.get(), &bucketUseOn);
    installed = true;
}

bool handleEvent(ItemUseEvent &event)
{
    if (const auto *player = WeakEntityRef(event.actor).tryUnwrap<::Player>(); player) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        const auto item = ItemStack(event.item_instance);
        endstone::PlayerInteractEvent e{
            player->getEndstoneActor<endstone::core::EndstonePlayer>(),
            endstone::PlayerInteractEvent::Action::RightClickAir,
            endstone::core::EndstoneItemStack::fromMinecraft(item),
            nullptr,
            endstone::BlockFace::South,
            std::nullopt,
        };
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }
    }
    return true;
}

bool handleEvent(ItemUseOnEvent &event)
{
    if (!event.is_first_event) {
        return true;
    }

    pending_bucket_fill.reset();

    auto *player = WeakEntityRef(event.actor).tryUnwrap<::Player>();
    if (!player) {
        return true;
    }

    const ::ItemStack item{event.item_before_use};
    const auto *minecraft_item = item.getItem();
    if (!minecraft_item || minecraft_item->getFullItemName() != "minecraft:bucket") {
        return true;
    }

    auto &block_source = player->getDimension().getBlockSourceFromMainChunkSource();
    const auto &block = block_source.getBlock(event.block_position);
    const auto bucket_name = getFilledBucketName(block);
    if (!bucket_name) {
        return true;
    }

    const auto block_face = endstone::core::EndstoneBlockFace::fromBedrockFacing(event.face);
    if (!block_face) {
        return true;
    }

    const auto &server = endstone::core::EndstoneServer::getInstance();
    auto block_clicked = endstone::core::EndstoneBlock::at(block_source, event.block_position);
    const auto hand = endstone::runtime::getInteractionHand(*player, item);
    auto bucket_stack = endstone::core::EndstoneItemStack::fromMinecraft(item);
    const auto &bucket_type = bucket_stack.getType();
    const auto result_item = ::ItemStack(*bucket_name);
    auto item_stack = endstone::core::EndstoneItemStack::fromMinecraft(result_item);
    endstone::PlayerBucketFillEvent bucket_event{
        player->getEndstoneActor<endstone::core::EndstonePlayer>(),
        block_clicked.get(),
        *block_clicked,
        *block_face,
        bucket_type,
        hand,
        std::move(item_stack),
    };
    server.getPluginManager().callEvent(bucket_event);
    if (bucket_event.isCancelled()) {
        return false;
    }

    const auto &event_item_stack = bucket_event.getItemStack();
    const auto keep_creative_result = player->getPlayerGameType() == GameType::Creative && event_item_stack &&
                                      endstone::core::EndstoneItemStack::toMinecraft(*event_item_stack) == result_item;
    pending_bucket_fill =
        PendingBucketFill{player, event.block_position, event_item_stack, !keep_creative_result};
    return true;
}

GameplayHandlerResult<CoordinatorResult> ScriptItemGameplayHandler::handleEvent2(
    MutableItemGameplayEvent<CoordinatorResult> &event)
{
    auto visitor = [&](auto &&arg) -> GameplayHandlerResult<CoordinatorResult> {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<ItemUseEvent>>) {
            if (!handleEvent(arg.value())) {
                return {HandlerResult::BypassListeners, CoordinatorResult::Cancel};
            }
        }
        else if constexpr (std::is_same_v<T, Details::ValueOrRef<ItemUseOnEvent>>) {
            if (!handleEvent(arg.value())) {
                return {HandlerResult::BypassListeners, CoordinatorResult::Cancel};
            }
        }
        return ENDSTONE_VHOOK_CALL_ORIGINAL(&ScriptItemGameplayHandler::handleEvent2, this, event);
    };
    return event.visit(visitor);
}
