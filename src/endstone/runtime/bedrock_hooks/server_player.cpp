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

#include "bedrock/server/server_player.h"

#include "bedrock/entity/components/experience_reward_component.h"
#include "bedrock/network/packet/death_info_packet.h"
#include "bedrock/world/level/storage/game_rules.h"
#include "endstone/core/damage/damage_source.h"
#include "endstone/core/player.h"
#include "endstone/core/server.h"
#include "endstone/event/player/player_death_event.h"
#include "endstone/message.h"
#include "endstone/runtime/hook.h"

void ServerPlayer::die(const ActorDamageSource &source)
{
    // Close any open form on player death
    auto &player = getEndstoneActor<endstone::core::EndstonePlayer>();
    player.closeForm();

    // Fire player death event
    auto &server = entt::locator<endstone::core::EndstoneServer>::value();
    auto death_cause_message = source.getDeathMessage(getName(), this);
    endstone::Message death_message = endstone::Translatable(death_cause_message.first, death_cause_message.second);
    endstone::PlayerDeathEvent e{player, std::make_unique<endstone::core::EndstoneDamageSource>(source),
                                 getOnDeathExperience(), death_message};
    server.getPluginManager().callEvent(static_cast<endstone::PlayerEvent &>(e));

    // Update death message
    death_message = e.getDeathMessage().value_or("");

    // Update dropped experience amount
    auto &exp_reward = getEntity().getOrAddComponent<ExperienceRewardComponent>();
    ExperienceRewardDefinition reward;
    reward.addDeathExpressionNode(ExpressionNode(e.getDroppedExp()));
    reward.initialize(getEntity(), exp_reward);

    // Send death info
    const auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::DeathInfo);
    const auto pk = std::static_pointer_cast<DeathInfoPacket>(packet);
    pk->death_cause_message = death_cause_message;
    sendNetworkPacket(*packet);

    // Broadcast death messages
    if (getLevel().getGameRules().getBool(GameRuleId(GameRules::SHOW_DEATH_MESSAGES), false) &&
        (!std::holds_alternative<std::string>(death_message) || !std::get<std::string>(death_message).empty())) {
        server.broadcastMessage(death_message);
    }

    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerPlayer::die, this, source);
}
