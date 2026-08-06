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

#include "endstone/runtime/bedrock_hooks/shear.h"

#include <cstddef>
#include <mutex>
#include <utility>
#include <vector>

#include "bedrock/entity/components/actor_equipment_component.h"
#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/actor_data_ids.h"
#include "bedrock/world/actor/actor_interaction.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/gamemode/interaction_result.h"
#include "bedrock/world/item/item.h"
#include "bedrock/world/item/item_stack.h"
#include "bedrock/world/level/block_source.h"
#include "bedrock/world/level/level.h"
#include "bedrock/world/level/spawner.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_shear_actor_event.h"
#include "endstone/runtime/bedrock_hooks/actor_interaction.h"
#include "endstone/runtime/vtable_hook.h"

namespace {
#ifdef _WIN32
constexpr int actor_get_interaction_ordinal = 109;
constexpr int spawner_spawn_item_ordinal = 9;
#else
constexpr int actor_get_interaction_ordinal = 118;
constexpr int spawner_spawn_item_ordinal = 10;
#endif

thread_local bool suppress_shear_drops = false;

struct ShearDropParameters {
    Vec3 position;
    int throw_time;
};

thread_local std::vector<ShearDropParameters> *suppressed_shear_drop_parameters = nullptr;

bool isShears(const ::ItemStack &item)
{
    return !item.isNull() && item.getItem() && item.getItem()->getFullItemName() == "minecraft:shears";
}

const ::ItemStack *getBodyItem(const ::Actor &actor)
{
    const auto *equipment = actor.tryGetComponent<ActorEquipmentComponent>();
    if (!equipment || !equipment->armor) {
        return nullptr;
    }
    return &equipment->armor->getItem(static_cast<int>(ArmorSlot::Body));
}

struct ShearDropSuppression {
    explicit ShearDropSuppression(std::vector<ShearDropParameters> &parameters)
        : previous_(suppress_shear_drops), previous_parameters_(suppressed_shear_drop_parameters)
    {
        suppress_shear_drops = true;
        suppressed_shear_drop_parameters = &parameters;
    }
    ~ShearDropSuppression()
    {
        suppress_shear_drops = previous_;
        suppressed_shear_drop_parameters = previous_parameters_;
    }

private:
    bool previous_;
    std::vector<ShearDropParameters> *previous_parameters_;
};

class ActorInteractionHook {
public:
    InteractionResult getInteraction(::Player &, ::ActorInteraction &, Vec3 const &);
};

class SpawnerHook {
public:
    ItemActor *spawnItem(::BlockSource &, ::ItemStack const &, ::Actor *, ::Vec3 const &, int);
};
}  // namespace

namespace endstone::runtime {

static bool isShearTarget(const ::Actor &target)
{
    const auto &name = target.getActorIdentifier().getCanonicalName();
    return name == "minecraft:sheep" || name == "minecraft:mooshroom" || name == "minecraft:snow_golem" ||
           name == "minecraft:bogged" || name == "minecraft:sulfur_cube" || name == "minecraft:wolf" ||
           name == "minecraft:happy_ghast";
}

static void getDefaultShearDrops(const ::Actor &target, std::vector<::ItemStack> &drops)
{
    const auto &name = target.getActorIdentifier().getCanonicalName();
    if (name == "minecraft:sheep") {
        auto color = 0;
        const auto color_id = static_cast<SynchedActorData::ID>(ActorDataIDs::COLOR_INDEX);
        if (target.entity_data.hasData(color_id)) {
            color = target.entity_data.getInt(color_id);
        }
        const auto count = target.getLevel().getRandom().nextInt(3) + 1;
        drops.reserve(drops.size() + count);
        for (auto i = 0; i < count; ++i) {
            drops.emplace_back("minecraft:wool", 1, color);
        }
        return;
    }

    if (name == "minecraft:mooshroom") {
        auto mushroom = "minecraft:red_mushroom";
        const auto variant_id = static_cast<SynchedActorData::ID>(ActorDataIDs::VARIANT);
        if (target.entity_data.hasData(variant_id) && target.entity_data.getInt(variant_id) == 1) {
            mushroom = "minecraft:brown_mushroom";
        }
        drops.reserve(drops.size() + 5);
        for (auto i = 0; i < 5; ++i) {
            drops.emplace_back(mushroom, 1);
        }
        return;
    }

    if (name == "minecraft:snow_golem") {
        drops.emplace_back("minecraft:carved_pumpkin", 1);
        return;
    }

    if (name == "minecraft:bogged") {
        drops.reserve(drops.size() + 2);
        for (auto i = 0; i < 2; ++i) {
            drops.emplace_back(target.getLevel().getRandom().nextBoolean() ? "minecraft:brown_mushroom"
                                                                             : "minecraft:red_mushroom",
                               1);
        }
        return;
    }

    if (name == "minecraft:sulfur_cube") {
        // Bedrock's sulfur_cube interaction explicitly drops slot.weapon.mainhand.
        const auto &carried_item = target.getCarriedItem();
        if (!carried_item.isNull()) {
            drops.push_back(carried_item);
        }
        return;
    }

    if (name == "minecraft:wolf" || name == "minecraft:happy_ghast") {
        if (const auto *body_item = getBodyItem(target); body_item && !body_item->isNull()) {
            drops.push_back(*body_item);
        }
    }
}

static bool fireShearActorEvent(const ::Player &player, const ::Actor &target, const ::ItemStack &item,
                                 std::vector<::ItemStack> &drops)
{
    std::vector<endstone::ItemStack> event_drops;
    event_drops.reserve(drops.size());
    for (const auto &drop : drops) {
        event_drops.push_back(endstone::core::EndstoneItemStack::fromMinecraft(drop));
    }

    const auto &server = endstone::core::EndstoneServer::getInstance();
    endstone::PlayerShearActorEvent event{
        player.getEndstoneActor<endstone::core::EndstonePlayer>(),
        target.getEndstoneActor(),
        getInteractionHand(player, item),
        endstone::core::EndstoneItemStack::fromMinecraft(item),
        std::move(event_drops),
    };
    server.getPluginManager().callEvent(event);
    if (event.isCancelled()) {
        return false;
    }

    drops.clear();
    drops.reserve(event.getDrops().size());
    for (const auto &drop : event.getDrops()) {
        drops.push_back(endstone::core::EndstoneItemStack::toMinecraft(drop));
    }
    return true;
}

}  // namespace endstone::runtime

namespace {

InteractionResult ActorInteractionHook::getInteraction(::Player &player, ::ActorInteraction &interaction,
                                                         Vec3 const &location)
{
    auto *target = reinterpret_cast<::Actor *>(this);
    const auto result = ENDSTONE_VHOOK_CALL_ORIGINAL(&ActorInteractionHook::getInteraction, this, player, interaction,
                                                     location);
    const auto &item = player.getCarriedItem();
    const auto *context = endstone::runtime::getActorInteractionContext();
    if (!result.isSuccessful() || !interaction.interaction || !context || context->player != &player ||
        context->target != target || !isShears(item) || !endstone::runtime::isShearTarget(*target)) {
        return result;
    }

    std::vector<::ItemStack> drops;
    endstone::runtime::getDefaultShearDrops(*target, drops);
    if (!endstone::runtime::fireShearActorEvent(player, *target, item, drops)) {
        interaction.interaction = brstd::move_only_function<void()>();
        // Bedrock has no separate PASS result; Failure prevents the captured interaction from executing.
        return InteractionResult::Failure();
    }

    auto native_interaction = std::move(interaction.interaction);
    auto *region = &target->getDimensionBlockSource();
    auto *spawner = &target->getLevel().getSpawner();
    const auto position = target->getPosition();
    interaction.interaction = brstd::move_only_function<void()>(
        [native_interaction = std::move(native_interaction), region, spawner, position,
         drops = std::move(drops)]() mutable {
            std::vector<ShearDropParameters> native_drop_parameters;
            {
                ShearDropSuppression suppression(native_drop_parameters);
                native_interaction();
            }
            const ShearDropParameters fallback{position, 10};
            for (std::size_t i = 0; i < drops.size(); ++i) {
                const auto &parameters = native_drop_parameters.empty()
                                             ? fallback
                                             : native_drop_parameters[i < native_drop_parameters.size()
                                                                          ? i
                                                                          : native_drop_parameters.size() - 1];
                const auto &drop = drops[i];
                if (!drop.isNull()) {
                    spawner->spawnItem(*region, drop, nullptr, parameters.position, parameters.throw_time);
                }
            }
        });
    return result;
}

ItemActor *SpawnerHook::spawnItem(::BlockSource &region, ::ItemStack const &item, ::Actor *spawner,
                                  ::Vec3 const &position, int throw_time)
{
    if (suppress_shear_drops) {
        if (suppressed_shear_drop_parameters) {
            suppressed_shear_drop_parameters->push_back({position, throw_time});
        }
        return nullptr;
    }
    return ENDSTONE_VHOOK_CALL_ORIGINAL(&SpawnerHook::spawnItem, this, region, item, spawner, position, throw_time);
}

}  // namespace

namespace endstone::runtime {

void installShearHooks(::Actor &target, ::Player &player)
{
    static std::once_flag installed;
    if (!isShearTarget(target) || !isShears(player.getCarriedItem())) {
        return;
    }

    // vhook patches the selected function process-wide. The supported actor vtables use the same
    // getInteraction implementation at this slot.
    std::call_once(installed, [&target] {
        endstone::runtime::vhook::create<actor_get_interaction_ordinal>(&target,
                                                                          &ActorInteractionHook::getInteraction);
        endstone::runtime::vhook::create<spawner_spawn_item_ordinal>(&target.getLevel().getSpawner(),
                                                                       &SpawnerHook::spawnItem);
    });
}

}  // namespace endstone::runtime
