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

#include "bedrock/world/actor/player/player.h"

#include "bedrock/entity/components/abilities_component.h"
#include "bedrock/entity/components/actor_game_type_component.h"
#include "bedrock/entity/components/player_component.h"
#include "bedrock/entity/utilities/synched_actor_data_access.h"
#include "bedrock/network/packet/available_commands_packet.h"
#include "bedrock/network/packet/mob_equipment_packet.h"
#include "bedrock/symbol.h"
#include "bedrock/textobject/text_object.h"
#include "bedrock/util/color_format.h"
#include "bedrock/world/actor/actor_flags.h"
#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "bedrock/world/level/level.h"

Player *Player::tryGetFromEntity(EntityContext &entity, const bool include_removed)
{
    if (!entity.hasComponent<PlayerComponent>()) {
        return nullptr;
    }
    return static_cast<Player *>(Actor::tryGetFromEntity(entity, include_removed));
}

bool Player::hasBedPosition() const
{
    if (player_respawn_point_.spawn_block_pos == BlockPos::MIN) {
        return false;
    }
    return player_respawn_point_.dimension == VanillaDimensions::Overworld;
}

const BlockPos &Player::getBedPosition() const
{
    return player_respawn_point_.spawn_block_pos;
}

const PlayerInventory &Player::getSupplies() const
{
    return *inventory_;
}

PlayerInventory &Player::getSupplies()
{
    return *inventory_;
}

const Container &Player::getInventory() const
{
    return inventory_->getContainer();
}

Container &Player::getInventory()
{
    return inventory_->getContainer();
}

int Player::getSelectedItemSlot() const
{
    return inventory_->getSelectedSlot().slot;
}

const ItemStack &Player::setSelectedSlot(int slot)
{
    if (isClientSide()) {
        return ItemStack::EMPTY_ITEM;
    }
    if (!inventory_->selectSlot(slot, CONTAINER_ID_INVENTORY)) {
        return ItemStack::EMPTY_ITEM;
    }
    const auto &container = inventory_->getContainer();
    const auto &item = container.getItem(slot);
    const auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::PlayerEquipment);
    const auto pk = std::static_pointer_cast<MobEquipmentPacket>(packet);
    pk->runtime_id = getRuntimeID();
    pk->item = item;
    pk->slot = slot;
    pk->selected_slot = slot;
    pk->container_id = CONTAINER_ID_INVENTORY;
    pk->slot_byte = slot;
    pk->selected_slot_byte = slot;
    pk->container_id_byte = CONTAINER_ID_INVENTORY;
    getDimension().sendPacketForEntity(*this, *pk, nullptr);
    return item;
}

const std::string &Player::getName() const
{
    return name_;
}

void Player::setPermissions(CommandPermissionLevel permission)
{
    const auto component = getPersistentComponent<AbilitiesComponent>();
    component->abilities.setCommandPermissions(permission);
}

void Player::setBedRespawnPosition(const BlockPos &bed_position)
{
    if (!setSpawnBlockRespawnPosition(bed_position, VanillaDimensions::Overworld)) {
        return;
    }
    TextObjectRoot object;
    object.addChild<TextObjectText>(ColorFormat::GRAY);
    object.addChild<TextObjectLocalizedText>("tile.bed.respawnSet");
    displayTextObjectMessage(object, "", "");
}

bool Player::canSleep() const
{
    if (isSpectator()) {
        return false;
    }
    return isAlive();
}

GameType Player::getPlayerGameType() const
{
    auto game_type = GameType::Undefined;
    if (auto *component = tryGetComponent<ActorGameTypeComponent>(); component) {
        game_type = component->game_type;
    }
    if (game_type == GameType::Default) {
        game_type = getLevel().getDefaultGameType();
    }
    return game_type;
}

PlayerPermissionLevel Player::getPlayerPermissionLevel() const
{
    return getAbilities().getPlayerPermissions();
}

LayeredAbilities &Player::getAbilities()
{
    auto component = getPersistentComponent<AbilitiesComponent>();
    return component->abilities;
}

LayeredAbilities const &Player::getAbilities() const
{
    auto component = getPersistentComponent<AbilitiesComponent>();
    return component->abilities;
}

bool Player::isEmoting() const
{
    return getStatusFlag(ActorFlags::EMOTING);
}

bool Player::isFlying() const
{
    return getAbilities().getBool(AbilitiesIndex::Flying);
}

int Player::getPlayerLevel() const
{
    return static_cast<int>(getAttribute("minecraft:player.level").getCurrentValue());
}

float Player::getLevelProgress() const
{
    return getAttribute("minecraft:player.experience").getCurrentValue();
}

int Player::getXpNeededForLevelRange(int start, int end)
{
    auto xp = 0;
    auto current = start;
    while (current < end) {
        if (current > 30) {
            xp += (9 * current - 138);
        }
        else if (current > 15) {
            xp += (5 * current - 38);
        }
        else {
            xp += (2 * current + 7);
        }
        ++current;
    }
    return xp;
}
