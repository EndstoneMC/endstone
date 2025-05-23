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

#include "bedrock/world/item/item.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_item_consume_event.h"
#include "endstone/runtime/vtable_hook.h"

namespace {

bool handleEvent(ItemCompleteUseEvent &event)
{
    auto &item = event.item_instance;
    const std::set<std::string> item_names{"minecraft:potion", "minecraft:milk_bucket", "minecraft:medicine"};
    if (item.getItem()->isFood() || item_names.contains(item.getItem()->getFullItemName())) {
        const auto item_stack =
            event.item_instance.isNull()
                ? nullptr
                : endstone::core::EndstoneItemStack::fromMinecraft(reinterpret_cast<ItemStack &>(event.item_instance));
        if (auto *player = const_cast<Player *>(event.actor.tryUnwrap<::Player>()); player) {
            auto &server = entt::locator<endstone::core::EndstoneServer>::value();
            endstone::PlayerItemConsumeEvent e{*endstone::core::EndstonePlayer::create(server, *player),
                                               item_stack.get()};
            server.getPluginManager().callEvent(e);
            if (e.isCancelled()) {
                return false;
            }
        }
    }
    return true;
}

}  // namespace

GameplayHandlerResult<CoordinatorResult> ScriptItemGameplayHandler::handleEvent2(
    MutableItemGameplayEvent<CoordinatorResult> &event)
{
    auto visitor = [&](auto &&arg) -> GameplayHandlerResult<CoordinatorResult> {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<ItemCompleteUseEvent>>) {
            if (!handleEvent(arg.value())) {
                if (!handleEvent(arg.value())) {
                    return {HandlerResult::BypassListeners, CoordinatorResult::Cancel};
                }
            }
        }
        return ENDSTONE_VHOOK_CALL_ORIGINAL(&ScriptItemGameplayHandler::handleEvent2, this, event);
    };
    return event.visit(visitor);
}
