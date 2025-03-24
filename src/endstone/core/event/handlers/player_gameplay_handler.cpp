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

#include "endstone/core/event/handlers/player_gameplay_handler.h"

#include "bedrock/entity/components/replay_state_component.h"
#include "bedrock/locale/i18n.h"
#include "bedrock/network/packet/death_info_packet.h"
#include "bedrock/network/packet/update_player_game_type_packet.h"
#include "bedrock/world/actor/actor.h"
#include "endstone/color_format.h"
#include "endstone/core/block/block.h"
#include "endstone/core/damage/damage_source.h"
#include "endstone/core/game_mode.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/json.h"
#include "endstone/core/message.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_death_event.h"
#include "endstone/event/player/player_emote_event.h"
#include "endstone/event/player/player_game_mode_change_event.h"
#include "endstone/event/player/player_interact_actor_event.h"
#include "endstone/event/player/player_interact_event.h"
#include "endstone/event/player/player_join_event.h"
#include "endstone/event/player/player_quit_event.h"
#include "endstone/event/player/player_respawn_event.h"

namespace endstone::core {

EndstonePlayerGameplayHandler::EndstonePlayerGameplayHandler(std::unique_ptr<PlayerGameplayHandler> handle)
    : handle_(std::move(handle))
{
}

HandlerResult EndstonePlayerGameplayHandler::handleEvent(const PlayerGameplayEvent<void> &event)
{
    auto visitor = [&](auto &&arg) -> HandlerResult {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<const PlayerDamageEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<const PlayerDisconnectEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<const PlayerFormResponseEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<const PlayerFormCloseEvent>> ||
                      // std::is_same_v<T, Details::ValueOrRef<const PlayerInitialSpawnEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<const ::PlayerRespawnEvent>> ||
                      std::is_same_v<T, Details::ValueOrRef<const ::PlayerEmoteEvent>>) {
            if (!handleEvent(arg.value())) {
                return HandlerResult::BypassListeners;
            }
        }
        return handle_->handleEvent(event);
    };
    return event.visit(visitor);
}

GameplayHandlerResult<CoordinatorResult> EndstonePlayerGameplayHandler::handleEvent(
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
        return handle_->handleEvent(event);
    };
    return event.visit(visitor);
}

GameplayHandlerResult<CoordinatorResult> EndstonePlayerGameplayHandler::handleEvent(
    MutablePlayerGameplayEvent<CoordinatorResult> &event)
{
    auto visitor = [&](auto &&arg) -> GameplayHandlerResult<CoordinatorResult> {
        using T = std::decay_t<decltype(arg)>;
        if constexpr (std::is_same_v<T, Details::ValueOrRef<::PlayerGameModeChangeEvent>>) {
            if (!handleEvent(arg.value())) {
                return {HandlerResult::BypassListeners, CoordinatorResult::Cancel};
            }
        }
        return handle_->handleEvent(event);
    };
    return event.visit(visitor);
}

std::unique_ptr<PlayerGameplayHandler> EndstonePlayerGameplayHandler::unwrap()
{
    return std::move(handle_);
}

bool EndstonePlayerGameplayHandler::handleEvent(const PlayerDamageEvent &event)
{
    if (auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>(); player) {
        auto &server = entt::locator<EndstoneServer>::value();
        auto &endstone_player = player->getEndstoneActor<EndstonePlayer>();

        if (!player->isAlive()) {
            // Close any open form on player death
            endstone_player.closeForm();

            // Fire player death event
            auto death_cause_message = event.damage_source->getDeathMessage(player->getName(), player);
            auto death_message = getI18n().get(death_cause_message.first, death_cause_message.second, nullptr);
            const auto e = std::make_unique<PlayerDeathEvent>(
                endstone_player, std::make_unique<EndstoneDamageSource>(*event.damage_source), death_message);
            server.getPluginManager().callEvent(*static_cast<PlayerEvent *>(e.get()));
            if (e->getDeathMessage() != death_message) {
                death_cause_message.first = e->getDeathMessage();
                death_cause_message.second.clear();
            }

            // Send death info
            const auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::DeathInfo);
            const auto pk = std::static_pointer_cast<DeathInfoPacket>(packet);
            pk->death_cause_message = death_cause_message;
            player->sendNetworkPacket(*packet);

            // Log death message to console if not empty
            if (e->getDeathMessage().empty()) {
                return true;
            }

            // Broadcast death messages
            if (player->getLevel().getGameRules().getBool(GameRuleId(GameRules::SHOW_DEATH_MESSAGES), false)) {
                server.broadcastMessage(Translatable{death_cause_message.first, death_cause_message.second});
            }
            else {
                server.getLogger().info(e->getDeathMessage());
            }
        }
    }
    return true;
}

bool EndstonePlayerGameplayHandler::handleEvent(const PlayerDisconnectEvent &event)
{
    if (auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>(); player) {
        const auto &server = entt::locator<EndstoneServer>::value();
        auto &endstone_player = player->getEndstoneActor<EndstonePlayer>();
        endstone_player.disconnect();

        Translatable tr{ColorFormat::Yellow + "%multiplayer.player.left", {endstone_player.getName()}};
        const std::string quit_message = EndstoneMessage::toString(tr);

        PlayerQuitEvent e{endstone_player, quit_message};
        server.getPluginManager().callEvent(e);

        if (e.getQuitMessage() != quit_message) {
            tr = Translatable{e.getQuitMessage(), {}};
        }

        if (!e.getQuitMessage().empty()) {
            for (const auto &online_player : server.getOnlinePlayers()) {
                online_player->sendMessage(tr);
            }
        }
    }
    return true;
}

bool EndstonePlayerGameplayHandler::handleEvent(const PlayerFormResponseEvent &event)
{
    if (auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>(); player) {
        player->getEndstoneActor<EndstonePlayer>().onFormResponse(event.form_id, event.form_response);
    }
    return true;
}

bool EndstonePlayerGameplayHandler::handleEvent(const PlayerFormCloseEvent &event)
{
    if (auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>(); player) {
        player->getEndstoneActor<EndstonePlayer>().onFormClose(event.form_id, event.form_close_reason);
    }
    return true;
}

bool EndstonePlayerGameplayHandler::handleEvent(const ::PlayerRespawnEvent &event)
{
    if (const auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>(); player) {
        const auto &server = entt::locator<EndstoneServer>::value();
        PlayerRespawnEvent e{player->getEndstoneActor<EndstonePlayer>()};
        server.getPluginManager().callEvent(e);
    }
    return true;
}

bool EndstonePlayerGameplayHandler::handleEvent(const ::PlayerEmoteEvent &event)
{
    if (const auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>(); player) {
        const auto &server = entt::locator<EndstoneServer>::value();
        PlayerEmoteEvent e{player->getEndstoneActor<EndstonePlayer>(), event.emote_piece_id};
        server.getPluginManager().callEvent(e);
    }
    return true;
}

bool EndstonePlayerGameplayHandler::handleEvent(const PlayerInteractWithBlockBeforeEvent &event)
{
    if (const auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>(); player) {
        const auto &server = entt::locator<EndstoneServer>::value();
        auto &block_source = player->getDimension().getBlockSourceFromMainChunkSource();
        const auto block = EndstoneBlock::at(block_source, BlockPos(event.block_location));
        const auto item_stack = event.item.isNull() ? nullptr : EndstoneItemStack::fromMinecraft(event.item);

        PlayerInteractEvent e{
            player->getEndstoneActor<EndstonePlayer>(),
            item_stack.get(),
            block.get(),
            static_cast<BlockFace>(event.block_face),
            {event.face_location.x, event.face_location.y, event.face_location.z},
        };
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }
    }
    return true;
}

bool EndstonePlayerGameplayHandler::handleEvent(const PlayerInteractWithEntityBeforeEvent &event)
{
    const auto *player = WeakEntityRef(event.player).tryUnwrap<::Player>();
    const auto *target = WeakEntityRef(event.target_entity).tryUnwrap<::Actor>();

    if (player && target) {
        const auto &server = entt::locator<EndstoneServer>::value();
        PlayerInteractActorEvent e{player->getEndstoneActor<EndstonePlayer>(), target->getEndstoneActor()};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }
    }
    return true;
}

bool EndstonePlayerGameplayHandler::handleEvent(::PlayerGameModeChangeEvent &event)
{
    if (auto *player = event.player.tryUnwrap<::Player>(); player) {
        const auto &server = entt::locator<EndstoneServer>::value();
        PlayerGameModeChangeEvent e{player->getEndstoneActor<EndstonePlayer>(),
                                    EndstoneGameMode::fromMinecraft(event.to_game_mode)};
        server.getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            const auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::UpdatePlayerGameType);
            const auto pk = std::static_pointer_cast<UpdatePlayerGameTypePacket>(packet);
            pk->player_game_type = event.from_game_mode;
            pk->target_player = player->getOrCreateUniqueID();
            pk->tick = 0;
            if (const auto *component = player->tryGetComponent<ReplayStateComponent>(); component) {
                pk->tick = component->getCurrentTick();
            }
            server.getServer().getPacketSender().sendBroadcast(*pk);
            return false;
        }
    }
    return true;
}

}  // namespace endstone::core
