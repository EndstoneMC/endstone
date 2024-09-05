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

#include <utility>

#include "bedrock/locale/i18n.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/server.h"
#include "endstone/event/player/player_death_event.h"
#include "endstone/event/player/player_join_event.h"
#include "endstone/event/player/player_quit_event.h"

using endstone::detail::EndstoneServer;

namespace {
class ActorDamageSourceWrapper : public ActorDamageSource {
public:
    ActorDamageSourceWrapper(const ActorDamageSource &source, std::string message, std::vector<std::string> params)
        : source_(source), message_(std::move(message)), params_(std::move(params))
    {
    }

    ~ActorDamageSourceWrapper() override = default;

    [[nodiscard]] bool isEntitySource() const override
    {
        return source_.isEntitySource();
    }
    [[nodiscard]] bool isChildEntitySource() const override
    {
        return source_.isChildEntitySource();
    }
    [[nodiscard]] bool isBlockSource() const override
    {
        return source_.isBlockSource();
    }
    [[nodiscard]] bool isFire() const override
    {
        return source_.isFire();
    }
    [[nodiscard]] bool isReducedByResistanceEffect() const override
    {
        return source_.isReducedByResistanceEffect();
    }
    [[nodiscard]] bool isReducedByEnchantReduction() const override
    {
        return source_.isReducedByEnchantReduction();
    }
    [[nodiscard]] bool isReducedByArmorReduction() const override
    {
        return source_.isReducedByArmorReduction();
    }
    [[nodiscard]] bool isFallingBlockDamage() const override
    {
        return source_.isFallingBlockDamage();
    }
    [[nodiscard]] bool isFallDamage() const override
    {
        return source_.isFallDamage();
    }
    [[nodiscard]] std::pair<std::string, std::vector<std::string>> getDeathMessage(std::string,
                                                                                   Actor *actor) const override
    {
        return std::make_pair(message_, params_);
    }
    [[nodiscard]] bool getIsCreative() const override
    {
        return source_.getIsCreative();
    }
    [[nodiscard]] bool getIsWorldBuilder() const override
    {
        return source_.getIsWorldBuilder();
    }
    [[nodiscard]] ActorUniqueID getEntityUniqueID() const override
    {
        return source_.getEntityUniqueID();
    }
    [[nodiscard]] ActorType getEntityType() const override
    {
        return source_.getEntityType();
    }
    [[nodiscard]] ActorCategory getEntityCategories() const override
    {
        return source_.getEntityCategories();
    }
    [[nodiscard]] bool getDamagingEntityIsCreative() const override
    {
        return source_.getDamagingEntityIsCreative();
    }
    [[nodiscard]] bool getDamagingEntityIsWorldBuilder() const override
    {
        return source_.getDamagingEntityIsWorldBuilder();
    }
    [[nodiscard]] ActorUniqueID getDamagingEntityUniqueID() const override
    {
        return source_.getDamagingEntityUniqueID();
    }
    [[nodiscard]] ActorType getDamagingEntityType() const override
    {
        return source_.getDamagingEntityType();
    }
    [[nodiscard]] ActorCategory getDamagingEntityCategories() const override
    {
        return source_.getDamagingEntityCategories();
    }
    [[nodiscard]] std::unique_ptr<ActorDamageSource> clone() const override
    {
        return std::make_unique<ActorDamageSourceWrapper>(source_, message_, params_);
    }

private:
    const ActorDamageSource &source_;
    std::string message_;
    std::vector<std::string> params_;
};
}  // namespace

void ServerPlayer::die(const ActorDamageSource &source)
{
    auto death_cause_message = source.getDeathMessage(getName(), this);
    auto death_message = getI18n().get(death_cause_message.first, death_cause_message.second, nullptr);

    auto &server = entt::locator<EndstoneServer>::value();
    endstone::Player &endstone_player = getEndstonePlayer();
    endstone_player.closeForm();

    auto e = std::make_unique<endstone::PlayerDeathEvent>(endstone_player, death_message);
    server.getPluginManager().callEvent(*static_cast<endstone::PlayerEvent *>(e.get()));

    if (!e->getDeathMessage().empty()) {
        server.getLogger().info(e->getDeathMessage());
        if (e->getDeathMessage() != death_message) {
            auto new_source = ActorDamageSourceWrapper(source, e->getDeathMessage(), {});
            ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&ServerPlayer::die, __FUNCDNAME__, this, new_source);
            return;
        }
    }
    ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&ServerPlayer::die, __FUNCDNAME__, this, source);
}

void ServerPlayer::setLocalPlayerAsInitialized()
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerPlayer::setLocalPlayerAsInitialized, this);
    auto &server = entt::locator<EndstoneServer>::value();
    auto &endstone_player = getEndstonePlayer();
    endstone::PlayerJoinEvent e{endstone_player};
    server.getPluginManager().callEvent(e);
    endstone_player.recalculatePermissions();
    endstone_player.updateCommands();
}

void ServerPlayer::disconnect()
{
    auto &server = entt::locator<EndstoneServer>::value();
    auto &endstone_player = getEndstonePlayer();
    endstone_player.disconnect();
    endstone::PlayerQuitEvent e{endstone_player};
    server.getPluginManager().callEvent(e);
    ENDSTONE_HOOK_CALL_ORIGINAL(&ServerPlayer::disconnect, this);
}
