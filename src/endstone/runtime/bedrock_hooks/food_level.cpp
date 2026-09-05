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

#include <algorithm>
#include <optional>
#include <utility>

#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/attribute/attribute_instance.h"
#include "bedrock/world/attribute/hunger_attribute_delegate.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/actor/food_level_change_event.h"
#include "endstone/runtime/hook.h"

namespace {

struct AttributeContext;
struct ItemContext;
struct EatContext;
struct HungerNotificationContext;

thread_local AttributeContext *current_attribute_context = nullptr;
thread_local ItemContext *current_item_context = nullptr;
thread_local EatContext *current_eat_context = nullptr;
thread_local HungerNotificationContext *current_hunger_notification_context = nullptr;

struct AttributeContext {
    explicit AttributeContext(AttributeInstance &instance)
        : instance(&instance), previous(std::exchange(current_attribute_context, this))
    {
    }

    ~AttributeContext() { current_attribute_context = previous; }

    AttributeInstance *instance;
    bool food_event = false;
    AttributeContext *previous;
};

struct ItemContext {
    ItemContext(Player &player, const ItemStack &item)
        : player(&player), item(&item), previous(std::exchange(current_item_context, this))
    {
    }

    ~ItemContext() { current_item_context = previous; }

    Player *player;
    const ItemStack *item;
    bool consumed = false;
    ItemContext *previous;
};

struct HungerNotificationContext {
    explicit HungerNotificationContext(Player &player)
        : player(&player), previous(std::exchange(current_hunger_notification_context, this))
    {
    }

    ~HungerNotificationContext() { current_hunger_notification_context = previous; }

    Player *player;
    bool consumed = false;
    HungerNotificationContext *previous;
};

struct EatContext {
    EatContext(Player &player, int nutrition)
        : player(&player), nutrition(nutrition), previous(std::exchange(current_eat_context, this))
    {
    }

    ~EatContext() { current_eat_context = previous; }

    Player *player;
    int nutrition;
    bool consumed = false;
    EatContext *previous;
};

[[nodiscard]] bool has_food_level_change_listeners()
{
    const auto &server = endstone::core::EndstoneServer::getInstance();
    return server.getEndstonePluginManager().isEventRegistered<endstone::FoodLevelChangeEvent>();
}

[[nodiscard]] const HashedString &get_hunger_attribute_name()
{
    static const HashedString name{"minecraft:player.hunger"};
    return name;
}

}  // namespace

void Player::eat(const ItemStack &item)
{
    using EatItem = void (Player::*)(const ItemStack &);
    constexpr auto original = static_cast<EatItem>(&Player::eat);

    if (!has_food_level_change_listeners()) {
        ENDSTONE_HOOK_CALL_ORIGINAL(original, this, item);
        return;
    }

    ItemContext context{*this, item};
    ENDSTONE_HOOK_CALL_ORIGINAL(original, this, item);
}

void Player::eat(int nutrition, float saturation_modifier)
{
    using EatValues = void (Player::*)(int, float);
    constexpr auto original = static_cast<EatValues>(&Player::eat);

    if (!has_food_level_change_listeners()) {
        ENDSTONE_HOOK_CALL_ORIGINAL(original, this, nutrition, saturation_modifier);
        return;
    }

    const auto *hunger = getAttribute(get_hunger_attribute_name());
    if (!hunger) {
        ENDSTONE_HOOK_CALL_ORIGINAL(original, this, nutrition, saturation_modifier);
        return;
    }

    const auto old_food_level = static_cast<int>(hunger->getCurrentValue());
    std::optional<endstone::ItemStack> item;
    if (current_item_context && current_item_context->player == this && !current_item_context->consumed) {
        current_item_context->consumed = true;
        item = endstone::core::EndstoneItemStack::fromMinecraft(*current_item_context->item);
    }

    const auto from_item = item.has_value();
    const auto &server = endstone::core::EndstoneServer::getInstance();
    endstone::FoodLevelChangeEvent event{getEndstoneActor<endstone::core::EndstonePlayer>(), old_food_level + nutrition,
                                         std::move(item)};
    server.getPluginManager().callEvent(event);

    const auto applied_nutrition = event.isCancelled() ? 0 : event.getFoodLevel() - old_food_level;
    const auto native_nutrition = !event.isCancelled() && from_item ? nutrition : applied_nutrition;
    EatContext context{*this, applied_nutrition};
    ENDSTONE_HOOK_CALL_ORIGINAL(original, this, native_nutrition, saturation_modifier);
    if (auto mutable_hunger = getMutableAttribute(get_hunger_attribute_name())) {
        mutable_hunger->setCurrentValue(mutable_hunger->getCurrentValue());
    }
}

void HungerAttributeDelegate::notify(AttributeMessageType type, AttributeModificationContext &context)
{
    auto *player = getPlayer();
    if (type != 0 || !player || !has_food_level_change_listeners()) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&HungerAttributeDelegate::notify, this, type, context);
        return;
    }

    HungerNotificationContext notification_context{*player};
    ENDSTONE_HOOK_CALL_ORIGINAL(&HungerAttributeDelegate::notify, this, type, context);
}

std::optional<float> AttributeInstance::addBuff(const AttributeBuff &buff, AttributeModificationContext context)
{
    if (!current_eat_context && !has_food_level_change_listeners()) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&AttributeInstance::addBuff, this, buff, context);
    }

    AttributeContext attribute_context{*this};
    auto result = ENDSTONE_HOOK_CALL_ORIGINAL(&AttributeInstance::addBuff, this, buff, context);
    if (attribute_context.food_event) {
        setCurrentValue(getCurrentValue(), context);
    }
    return result;
}

void AttributeInstance::tick(AttributeModificationContext context)
{
    if (!has_food_level_change_listeners()) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&AttributeInstance::tick, this, context);
        return;
    }

    AttributeContext attribute_context{*this};
    ENDSTONE_HOOK_CALL_ORIGINAL(&AttributeInstance::tick, this, context);
    if (attribute_context.food_event) {
        setCurrentValue(getCurrentValue(), context);
    }
}

std::optional<float> AttributeInstanceDelegate::change(float old_value, float new_value, const AttributeBuff &buff)
{
    auto *attribute_context = current_attribute_context;
    if (!attribute_context || attribute_context->instance->getDelegate() != this) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&AttributeInstanceDelegate::change, this, old_value, new_value, buff);
    }

    auto &instance = *attribute_context->instance;
    const auto *attribute = instance.getAttribute();
    if (!attribute || attribute->getName() != get_hunger_attribute_name()) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&AttributeInstanceDelegate::change, this, old_value, new_value, buff);
    }

    auto *player = static_cast<const HungerAttributeDelegate *>(this)->getPlayer();
    if (!player) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&AttributeInstanceDelegate::change, this, old_value, new_value, buff);
    }

    if (current_eat_context && current_eat_context->player == player && !current_eat_context->consumed) {
        current_eat_context->consumed = true;
        return old_value + static_cast<float>(current_eat_context->nutrition);
    }

    auto *notification_context = current_hunger_notification_context;
    const auto from_exhaustion =
        notification_context && notification_context->player == player && !notification_context->consumed;
    if ((!from_exhaustion && buff.getType() != AttributeBuffType::Saturation) || !has_food_level_change_listeners()) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&AttributeInstanceDelegate::change, this, old_value, new_value, buff);
    }
    if (from_exhaustion) {
        notification_context->consumed = true;
    }

    instance.setCurrentValue(old_value, AttributeModificationContext{nullptr});
    const auto old_food_level = static_cast<int>(old_value);
    const auto proposed_food_level = std::max(static_cast<int>(new_value), 0);

    const auto &server = endstone::core::EndstoneServer::getInstance();
    endstone::FoodLevelChangeEvent event{player->getEndstoneActor<endstone::core::EndstonePlayer>(),
                                         proposed_food_level, std::nullopt};
    server.getPluginManager().callEvent(event);
    attribute_context->food_event = true;
    if (event.isCancelled()) {
        return instance.getCurrentValue();
    }

    if (from_exhaustion) {
        return static_cast<float>(event.getFoodLevel());
    }
    return instance.getCurrentValue() + static_cast<float>(event.getFoodLevel() - old_food_level);
}
