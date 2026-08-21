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

#include "endstone/runtime/bedrock_hooks/sign_item.h"

#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/item/registry/item_registry_manager.h"
#include "endstone/core/player_open_sign.h"
#include "endstone/core/server.h"
#include "endstone/runtime/vtable_hook.h"

namespace {

#ifdef _WIN32
constexpr int use_on_ordinal = 129;
#else
constexpr int use_on_ordinal = 130;
#endif

void clearPendingOpenSignCause(::Actor &actor)
{
    if (actor.isPlayer()) {
        endstone::core::clearPendingOpenSignCause(static_cast<::Player &>(actor));
    }
}

InteractionResult signItemUseOn(const ::Item *item, ::ItemStack &item_stack, ::Actor &actor, BlockPos position,
                                FacingID face, const Vec3 &click_pos)
{
    const auto result =
        ENDSTONE_VHOOK_CALL_ORIGINAL(&signItemUseOn, item, item_stack, actor, position, face, click_pos);
    clearPendingOpenSignCause(actor);
    return result;
}

InteractionResult hangingSignItemUseOn(const ::Item *item, ::ItemStack &item_stack, ::Actor &actor, BlockPos position,
                                       FacingID face, const Vec3 &click_pos)
{
    const auto result =
        ENDSTONE_VHOOK_CALL_ORIGINAL(&hangingSignItemUseOn, item, item_stack, actor, position, face, click_pos);
    clearPendingOpenSignCause(actor);
    return result;
}

void *getUseOnTarget(::Item *item)
{
    return (*reinterpret_cast<void ***>(item))[use_on_ordinal];
}

}  // namespace

void endstone::runtime::installSignItemHook()
{
    static bool installed = false;
    if (installed) {
        return;
    }

    const auto &server = endstone::core::EndstoneServer::getInstance();
    const auto registry = ItemRegistryManager::getItemRegistry();
    if (!registry.isValid()) {
        server.getLogger().error("Unable to hook SignItem::_useOn: the item registry is not available.");
        return;
    }

    int aux_value = 0;
    auto sign = registry.lookupByName(aux_value, "minecraft:oak_sign");
    if (sign.isNull()) {
        sign = registry.lookupByName(aux_value, "minecraft:sign");
    }
    const auto hanging_sign = registry.lookupByName(aux_value, "minecraft:oak_hanging_sign");
    if (sign.isNull() && hanging_sign.isNull()) {
        server.getLogger().error("Unable to hook SignItem::_useOn: no sign item found in the item registry.");
        return;
    }

    const auto sign_target = sign.isNull() ? nullptr : getUseOnTarget(sign.get());
    if (sign_target != nullptr) {
        endstone::runtime::vhook::create<use_on_ordinal>(sign.get(), &signItemUseOn);
    }
    if (!hanging_sign.isNull() && getUseOnTarget(hanging_sign.get()) != sign_target) {
        endstone::runtime::vhook::create<use_on_ordinal>(hanging_sign.get(), &hangingSignItemUseOn);
    }
    installed = true;
}
