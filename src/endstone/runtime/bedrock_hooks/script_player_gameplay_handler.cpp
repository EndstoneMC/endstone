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

#include "bedrock/scripting/event_handlers/script_player_gameplay_handler.h"

#include <string_view>

#include "../../../../include/endstone/event/actor/player_death_event.h"
#include "bedrock/entity/components/replay_state_component.h"
#include "bedrock/locale/i18n.h"
#include "bedrock/network/packet/death_info_packet.h"
#include "bedrock/network/packet/update_player_game_type_packet.h"
#include "bedrock/world/actor/actor.h"
#include "bedrock/world/actor/item/item_actor.h"
#include "endstone/color_format.h"
#include "endstone/core/block/block.h"
#include "endstone/core/damage/damage_source.h"
#include "endstone/core/game_mode.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/json.h"
#include "endstone/core/message.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_dimension_change_event.h"
#include "endstone/event/player/player_drop_item_event.h"
#include "endstone/event/player/player_emote_event.h"
#include "endstone/event/player/player_game_mode_change_event.h"
#include "endstone/event/player/player_interact_actor_event.h"
#include "endstone/event/player/player_interact_event.h"
#include "endstone/event/player/player_quit_event.h"
#include "endstone/event/player/player_respawn_event.h"
#include "endstone/runtime/vtable_hook.h"

namespace {
bool handleEvent(const PlayerDamageEvent &event)
{
    if (auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>(); player) {
        auto &server = endstone::core::EndstoneServer::getInstance();
        auto &endstone_player = player->getEndstoneActor<endstone::core::EndstonePlayer>();

        if (!player->isAlive()) {
            // Close any open form on player death
            endstone_player.closeForm();

            // Fire player death event
            auto death_cause_message = event.damage_source->getDeathMessage(player->getName(), player);
            endstone::Message death_message =
                endstone::Translatable(death_cause_message.first, death_cause_message.second);
            endstone::PlayerDeathEvent e{endstone_player,
                                         std::make_unique<endstone::core::EndstoneDamageSource>(*event.damage_source),
                                         death_message};
            server.getPluginManager().callEvent(e);
            death_message = e.getDeathMessage().value_or("");

            // Send death info
            // TODO(refactor): use ActorDamageSource::setDeathMessageOverride
            const auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::DeathInfo);
            auto &pk = static_cast<DeathInfoPacket &>(*packet);
            auto death_message_tr = endstone::core::EndstoneMessage::toTranslatable(death_message);
            pk.payload.death_cause_message = {{death_message_tr.getText(), death_message_tr.getParameters()}};
            player->sendNetworkPacket(pk);

            // Broadcast death messages
            if (player->getLevel().getGameRules().getBool(GameRuleId(GameRules::SHOW_DEATH_MESSAGES), false) &&
                (!std::holds_alternative<std::string>(death_message) ||
                 !std::get<std::string>(death_message).empty())) {
                server.broadcastMessage(death_message);
            }
        }
    }
    return true;
}

bool handleEvent(const PlayerDisconnectEvent &event)
{
    if (auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>(); player) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        auto &endstone_player = player->getEndstoneActor<endstone::core::EndstonePlayer>();
        endstone_player.disconnect();

        endstone::Message quit_message = endstone::Translatable{
            endstone::ColorFormat::Yellow + "%multiplayer.player.left", {endstone_player.getName()}};
        endstone::PlayerQuitEvent e{endstone_player, quit_message};
        server.getPluginManager().callEvent(e);

        quit_message = e.getQuitMessage().value_or("");
        if (server.getServer().getServerTextSettings()->getEnabledServerTextEvents().test(
                static_cast<std::underlying_type_t<ServerTextEvent>>(ServerTextEvent::PlayerConnection)) &&
            (!std::holds_alternative<std::string>(quit_message) || !std::get<std::string>(quit_message).empty())) {
            server.broadcastMessage(quit_message);
        }
    }
    return true;
}

bool handleEvent(const PlayerFormResponseEvent &event)
{
    if (auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>(); player) {
        player->getEndstoneActor<endstone::core::EndstonePlayer>().onFormResponse(event.form_id, event.form_response);
    }
    return true;
}

bool handleEvent(const PlayerFormCloseEvent &event)
{
    if (auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>(); player) {
        player->getEndstoneActor<endstone::core::EndstonePlayer>().onFormClose(event.form_id, event.form_close_reason);
    }
    return true;
}

bool handleEvent(const ::PlayerRespawnEvent &event)
{
    if (const auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>(); player) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        endstone::PlayerRespawnEvent e{player->getEndstoneActor<endstone::core::EndstonePlayer>()};
        server.getPluginManager().callEvent(e);
    }
    return true;
}

bool handleEvent(const PlayerDimensionChangeAfterEvent &event)
{
    if (const auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>(); player) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        endstone::PlayerDimensionChangeEvent e{
            player->getEndstoneActor<endstone::core::EndstonePlayer>(),
            *server.getEndstoneLevel()->getDimension(event.from_dimension.runtime_id),
            *server.getEndstoneLevel()->getDimension(event.to_dimension.runtime_id),
        };
        server.getPluginManager().callEvent(e);
    }
    return true;
}

bool handleEvent(const PlayerInteractWithBlockBeforeEvent &event)
{
    if (const auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>(); player) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        auto &block_source = player->getDimension().getBlockSourceFromMainChunkSource();
        const auto block = endstone::core::EndstoneBlock::at(block_source, BlockPos(event.block_location));
        std::optional<endstone::ItemStack> item_stack;
        if (!event.item.isNull()) {
            item_stack = endstone::core::EndstoneItemStack::fromMinecraft(event.item);
        }
        endstone::PlayerInteractEvent e{
            player->getEndstoneActor<endstone::core::EndstonePlayer>(),
            endstone::PlayerInteractEvent::Action::RightClickBlock,
            std::move(item_stack),
            block.get(),
            static_cast<endstone::BlockFace>(event.block_face),
            endstone::Vector{event.face_location.x, event.face_location.y, event.face_location.z},
        };
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }
    }
    return true;
}

bool handleEvent(const PlayerInteractWithEntityBeforeEvent &event)
{
    const auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>();
    const auto *target = WeakEntityRef(event.target_entity).tryUnwrap<::Actor>();

    if (player && target) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        endstone::PlayerInteractActorEvent e{player->getEndstoneActor<endstone::core::EndstonePlayer>(),
                                             target->getEndstoneActor()};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }
    }
    return true;
}

bool handleEvent(::PlayerGameModeChangeEvent &event)
{
    if (auto *player = event.player.tryUnwrap<::Player>(); player) {
        const auto &server = endstone::core::EndstoneServer::getInstance();
        endstone::PlayerGameModeChangeEvent e{player->getEndstoneActor<endstone::core::EndstonePlayer>(),
                                              endstone::core::EndstoneGameMode::fromMinecraft(event.to_game_mode)};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            const auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::UpdatePlayerGameType);
            const auto pk = std::static_pointer_cast<UpdatePlayerGameTypePacket>(packet);
            pk->payload.player_game_type = event.from_game_mode;
            pk->payload.target_player = player->getOrCreateUniqueID();
            pk->payload.tick = 0;
            if (const auto *component = player->tryGetComponent<ReplayStateComponent>(); component) {
                pk->payload.tick = component->getCurrentTick();
            }
            server.getServer().getPacketSender().sendBroadcast(*pk);
            return false;
        }
    }
    return true;
}
}  // namespace

HandlerResult ScriptPlayerGameplayHandler::handleEvent1(const PlayerGameplayEvent<void> &event)
{
    auto visitor = [&](auto &&arg) -> HandlerResult {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<const PlayerDamageEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<const PlayerDisconnectEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<const PlayerFormResponseEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<const PlayerFormCloseEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<const ::PlayerRespawnEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<const PlayerDimensionChangeAfterEvent>>) {
            if (!handleEvent(arg.value())) {
                return HandlerResult::BypassListeners;
            }
        }
        return ENDSTONE_VHOOK_CALL_ORIGINAL(&ScriptPlayerGameplayHandler::handleEvent1, this, event);
    };
    return event.visit(visitor);
}

GameplayHandlerResult<CoordinatorResult> ScriptPlayerGameplayHandler::handleEvent2(
    const PlayerGameplayEvent<CoordinatorResult> &event)
{
    auto visitor = [&](auto &&arg) -> GameplayHandlerResult<CoordinatorResult> {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<const PlayerInteractWithBlockBeforeEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<const PlayerInteractWithEntityBeforeEvent>>) {
            if (!handleEvent(arg.value())) {
                return {HandlerResult::BypassListeners, CoordinatorResult::Cancel};
            }
        }
        return ENDSTONE_VHOOK_CALL_ORIGINAL(&ScriptPlayerGameplayHandler::handleEvent2, this, event);
    };
    return event.visit(visitor);
}

GameplayHandlerResult<CoordinatorResult> ScriptPlayerGameplayHandler::handleEvent3(
    MutablePlayerGameplayEvent<CoordinatorResult> &event)
{
    auto visitor = [&](auto &&arg) -> GameplayHandlerResult<CoordinatorResult> {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<::PlayerGameModeChangeEvent>>) {
            if (!handleEvent(arg.value())) {
                return {HandlerResult::BypassListeners, CoordinatorResult::Cancel};
            }
        }
        return ENDSTONE_VHOOK_CALL_ORIGINAL(&ScriptPlayerGameplayHandler::handleEvent3, this, event);
    };
    return event.visit(visitor);
}
