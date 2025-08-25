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

#include "endstone/core/player.h"

#include <magic_enum/magic_enum.hpp>

#include "bedrock/deps/raknet/rak_peer_interface.h"
#include "bedrock/entity/components/user_entity_identifier_component.h"
#include "bedrock/network/packet.h"
#include "bedrock/network/packet/emote_packet.h"
#include "bedrock/network/packet/mob_equipment_packet.h"
#include "bedrock/network/packet/modal_form_request_packet.h"
#include "bedrock/network/packet/play_sound_packet.h"
#include "bedrock/network/packet/player_auth_input_packet.h"
#include "bedrock/network/packet/player_skin_packet.h"
#include "bedrock/network/packet/set_title_packet.h"
#include "bedrock/network/packet/stop_sound_packet.h"
#include "bedrock/network/packet/text_packet.h"
#include "bedrock/network/packet/toast_request_packet.h"
#include "bedrock/network/packet/transfer_packet.h"
#include "bedrock/network/packet/update_abilities_packet.h"
#include "bedrock/network/server_network_handler.h"
#include "bedrock/platform/build_platform.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/level/level.h"
#include "endstone/block/block.h"
#include "endstone/color_format.h"
#include "endstone/core/base64.h"
#include "endstone/core/form/form_codec.h"
#include "endstone/core/game_mode.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/inventory/player_inventory.h"
#include "endstone/core/message.h"
#include "endstone/core/network/data_packet.h"
#include "endstone/core/server.h"
#include "endstone/core/skin.h"
#include "endstone/core/util/socket_address.h"
#include "endstone/core/util/uuid.h"
#include "endstone/event/player/player_bed_leave_event.h"
#include "endstone/event/player/player_emote_event.h"
#include "endstone/event/player/player_interact_event.h"
#include "endstone/event/player/player_item_held_event.h"
#include "endstone/event/player/player_join_event.h"
#include "endstone/event/player/player_skin_change_event.h"
#include "endstone/form/action_form.h"
#include "endstone/form/message_form.h"

namespace endstone::core {

EndstonePlayer::EndstonePlayer(EndstoneServer &server, ::Player &player)
    : EndstoneMob(server, player), perm_(std::make_shared<PermissibleBase>(static_cast<Player *>(this))),
      inventory_(std::make_unique<EndstonePlayerInventory>(player)),
      ender_chest_(std::make_unique<EndstoneInventory>(*player.getEnderChestContainer()))
{
    const auto component = player.getPersistentComponent<UserEntityIdentifierComponent>();
    uuid_ = EndstoneUUID::fromMinecraft(component->getClientUUID());
    xuid_ = component->getXuid(false);
    last_op_status_ = EndstonePlayer::isOp();
}

Player *EndstonePlayer::asPlayer() const
{
    return const_cast<EndstonePlayer *>(this);
}

void EndstonePlayer::sendMessage(const Message &message) const
{
    std::visit(overloaded{[this](const std::string &msg) {
                              auto packet = TextPacket::createRaw(msg);
                              getPlayer().sendNetworkPacket(packet);
                          },
                          [this](const Translatable &msg) {
                              auto packet = TextPacket::createTranslated(msg.getText(), msg.getParameters());
                              getPlayer().sendNetworkPacket(packet);
                          }},
               message);
}

void EndstonePlayer::sendErrorMessage(const Message &message) const
{
    std::visit(overloaded{[this](const std::string &msg) { sendMessage(ColorFormat::Red + msg); },
                          [this](const Translatable &msg) {
                              Translatable tr{ColorFormat::Red + "%" + msg.getText(), msg.getParameters()};
                              sendMessage(tr);
                          }},
               message);
}

std::string EndstonePlayer::getName() const
{
    return getPlayer().getName();
}

PermissionLevel EndstonePlayer::getPermissionLevel() const
{
    return isOp() ? PermissionLevel::Operator : PermissionLevel::Default;
}

bool EndstonePlayer::isPermissionSet(std::string name) const
{
    return perm_->isPermissionSet(name);
}

bool EndstonePlayer::isPermissionSet(const Permission &perm) const
{
    return perm_->isPermissionSet(perm);
}

bool EndstonePlayer::hasPermission(std::string name) const
{
    return perm_->hasPermission(name);
}

bool EndstonePlayer::hasPermission(const Permission &perm) const
{
    return perm_->hasPermission(perm);
}

PermissionAttachment *EndstonePlayer::addAttachment(Plugin &plugin, const std::string &name, bool value)
{
    return perm_->addAttachment(plugin, name, value);
}

PermissionAttachment *EndstonePlayer::addAttachment(Plugin &plugin)
{
    return perm_->addAttachment(plugin);
}

void EndstonePlayer::sendPacket(int packet_id, std::string_view payload) const
{
    DataPacket pk(packet_id, payload);
    getPlayer().sendNetworkPacket(pk);
}

bool EndstonePlayer::handlePacket(Packet &packet)
{
    switch (packet.getId()) {
    case MinecraftPacketIds::PlayerEquipment: {
        auto &pk = static_cast<MobEquipmentPacket &>(packet);
        PlayerItemHeldEvent e(*this, this->inventory_->getHeldItemSlot(), pk.selected_slot);
        getServer().getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            this->inventory_->setHeldItemSlot(this->inventory_->getHeldItemSlot());
            return false;
        }
        return true;
    }
    case MinecraftPacketIds::PlayerAction: {
        auto &pk = static_cast<PlayerActionPacket &>(packet);
        if (pk.payload.action == PlayerActionType::StopSleeping && getPlayer().isSleeping()) {
            std::unique_ptr<Block> bed;
            if (getPlayer().hasBedPosition()) {
                const auto bed_position = getPlayer().getBedPosition();
                bed = getDimension().getBlockAt(bed_position.x, bed_position.y, bed_position.z);
            }
            else {
                bed = getDimension().getBlockAt(getLocation());
            }

            PlayerBedLeaveEvent e(*this, *bed);
            getServer().getPluginManager().callEvent(e);
        }
        return true;
    }
    case MinecraftPacketIds::PlayerSkin: {
        auto &server = static_cast<EndstoneServer &>(getServer());
        auto &pk = static_cast<PlayerSkinPacket &>(packet);
        if (getPlayer().getPersistentComponent<UserEntityIdentifierComponent>()->getClientUUID() == pk.uuid) {
            Message skin_change_message =
                Translatable(ColorFormat::Yellow + (pk.skin.getIsPersona() ? "%multiplayer.player.changeToPersona"
                                                                           : "%multiplayer.player.changeToSkin"),
                             {getName()});
            PlayerSkinChangeEvent e{*this, EndstoneSkin::fromMinecraft(pk.skin), skin_change_message};
            getServer().getPluginManager().callEvent(e);
            if (e.isCancelled()) {
                auto new_packet = MinecraftPackets::createPacket(MinecraftPacketIds::PlayerSkin);
                auto &new_pk = static_cast<PlayerSkinPacket &>(*new_packet);
                new_pk.uuid = pk.uuid;
                new_pk.skin = getPlayer().getSkin();
                new_pk.localized_new_skin_name = pk.localized_old_skin_name;
                new_pk.localized_old_skin_name = pk.localized_new_skin_name;
                getPlayer().sendNetworkPacket(new_pk);
                return false;
            }

            skin_change_message = e.getSkinChangeMessage().value_or("");
            if (server.getServer().getServerTextSettings()->getEnabledServerTextEvents().test(
                    static_cast<std::underlying_type_t<ServerTextEvent>>(ServerTextEvent::PlayerChangedSkin)) &&
                (!std::holds_alternative<std::string>(skin_change_message) ||
                 !std::get<std::string>(skin_change_message).empty())) {
                server.broadcastMessage(skin_change_message);
            }
        }
        return true;
    }
    case MinecraftPacketIds::SetLocalPlayerAsInit: {
        doFirstSpawn();
        return true;
    }
    case MinecraftPacketIds::Emote: {
        auto &pk = static_cast<EmotePacket &>(packet);
        if (pk.isServerSide()) {
            return true;
        }
        PlayerEmoteEvent e(*this, pk.piece_id, pk.isEmoteChatMuted());
        getServer().getPluginManager().callEvent(e);
        if (e.isCancelled()) {
            return false;
        }
        if (e.isMuted()) {
            pk.flags |= static_cast<uint8_t>(EmotePacket::Flags::MUTE_EMOTE_CHAT);
        }
        else {
            pk.flags &= ~static_cast<uint8_t>(EmotePacket::Flags::MUTE_EMOTE_CHAT);
        }
        return true;
    }
    case MinecraftPacketIds::PlayerAuthInputPacket: {
        auto &pk = static_cast<PlayerAuthInputPacket &>(packet);
        auto &actions = pk.player_block_actions.actions_;
        for (auto it = actions.begin(); it != actions.end();) {
            const auto &action = *it;
            if (action.player_action_type == PlayerActionType::StartDestroyBlock) {
                const auto item = getInventory().getItemInMainHand();
                const auto block = getDimension().getBlockAt(action.pos.x, action.pos.y, action.pos.z);
                PlayerInteractEvent e{
                    *this,
                    PlayerInteractEvent::Action::LeftClickBlock,
                    item.get(),
                    block.get(),
                    static_cast<BlockFace>(action.facing),
                    endstone::Vector<float>{static_cast<float>(action.pos.x), static_cast<float>(action.pos.y),
                                            static_cast<float>(action.pos.z)},
                };
                getServer().getPluginManager().callEvent(e);
                if (e.isCancelled()) {
                    it = actions.erase(it);
                    continue;
                }
            }
            ++it;
        }
        return true;
    }
    default:
        return true;
    }
}

Result<void> EndstonePlayer::removeAttachment(PermissionAttachment &attachment)
{
    return perm_->removeAttachment(attachment);
}

void EndstonePlayer::recalculatePermissions()
{
    perm_->recalculatePermissions();
}

std::unordered_set<PermissionAttachmentInfo *> EndstonePlayer::getEffectivePermissions() const
{
    return perm_->getEffectivePermissions();
}

Server &EndstonePlayer::getServer() const
{
    return EndstoneMob::getServer();
}

bool EndstonePlayer::isOp() const
{
    return getPlayer().getCommandPermissionLevel() > CommandPermissionLevel::Any;
}

void EndstonePlayer::setOp(bool value)
{
    if (value == isOp()) {
        return;
    }

    getPlayer().setPermissions(value ? CommandPermissionLevel::Admin : CommandPermissionLevel::Any);
    recalculatePermissions();
    updateCommands();
    last_op_status_ = value;
}

void EndstonePlayer::remove()
{
    getServer().getLogger().error("Cannot remove player {}, use Player::kick instead.", getName());
}

UUID EndstonePlayer::getUniqueId() const
{
    return uuid_;
}

std::string EndstonePlayer::getXuid() const
{
    return xuid_;
}

SocketAddress EndstonePlayer::getAddress() const
{
    auto component = getPlayer().getPersistentComponent<UserEntityIdentifierComponent>();
    return EndstoneSocketAddress::fromNetworkIdentifier(component->getNetworkId());
}

void EndstonePlayer::sendPopup(std::string message) const
{
    TextPacket packet;
    packet.type = TextPacketType::Popup;
    packet.message = std::move(message);
    getPlayer().sendNetworkPacket(packet);
}

void EndstonePlayer::sendTip(std::string message) const
{
    TextPacket packet;
    packet.type = TextPacketType::Tip;
    packet.message = std::move(message);
    getPlayer().sendNetworkPacket(packet);
}

void EndstonePlayer::sendToast(std::string title, std::string content) const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::ToastRequest);
    auto pk = std::static_pointer_cast<ToastRequestPacket>(packet);
    pk->payload.title = std::move(title);
    pk->payload.content = std::move(content);
    getPlayer().sendNetworkPacket(*packet);
}

void EndstonePlayer::kick(std::string message) const
{
    auto *component = getPlayer().tryGetComponent<UserEntityIdentifierComponent>();
    server_.getServer().getMinecraft()->getServerNetworkHandler()->disconnectClient(
        component->getNetworkId(), component->getSubClientId(), Connection::DisconnectFailReason::Kicked, message,
        std::nullopt, false);
}

void EndstonePlayer::giveExp(int amount)
{
    getPlayer().addExperience(amount);
}

void EndstonePlayer::giveExpLevels(int amount)
{
    getPlayer().addLevels(amount);
}

float EndstonePlayer::getExpProgress() const
{
    return getPlayer().getLevelProgress();
}

Result<void> EndstonePlayer::setExpProgress(float progress)
{
    ENDSTONE_CHECKF(progress >= 0.0 && progress <= 1.0,  //
                    "Experience progress must be between 0.0 and 1.0 ({})", progress);
    auto mutable_attr = getPlayer().getMutableAttribute("minecraft:player.experience");
    mutable_attr.instance->setCurrentValue(progress, mutable_attr.context);
    return {};
}

int EndstonePlayer::getExpLevel() const
{
    return getPlayer().getPlayerLevel();
}

Result<void> EndstonePlayer::setExpLevel(int level)
{
    ENDSTONE_CHECKF(level >= 0, "Experience level must not be negative ({})", level);
    giveExpLevels(level - getExpLevel());
    return {};
}

int EndstonePlayer::getTotalExp() const
{
    auto progress = getExpProgress();
    auto base = ::Player::getXpNeededForLevelRange(0, getExpLevel());
    auto xp_for_next_level = static_cast<float>(::Player::getXpNeededForLevelRange(getExpLevel(), getExpLevel() + 1));
    return base + static_cast<int>(std::round(progress * xp_for_next_level));
}

bool EndstonePlayer::getAllowFlight() const
{
    return getPlayer().getAbilities().getBool(AbilitiesIndex::MayFly);
}

void EndstonePlayer::setAllowFlight(bool flight)
{
    if (isFlying() && !flight) {
        getPlayer().getAbilities().setAbility(AbilitiesIndex::Flying, false);
    }

    getPlayer().getAbilities().setAbility(AbilitiesIndex::MayFly, flight);
    updateAbilities();
}

bool EndstonePlayer::isFlying() const
{
    return getPlayer().isFlying();
}

Result<void> EndstonePlayer::setFlying(bool value)
{
    if (!getAllowFlight()) {
        ENDSTONE_CHECKF(!value, "Player {} is not allowed to fly.", getName());
    }

    getPlayer().getAbilities().setAbility(AbilitiesIndex::Flying, value);
    updateAbilities();
    return {};
}

float EndstonePlayer::getFlySpeed() const
{
    return getPlayer().getAbilities().getFloat(AbilitiesIndex::FlySpeed);
}

void EndstonePlayer::setFlySpeed(float value) const
{
    getPlayer().getAbilities().setAbility(AbilitiesIndex::FlySpeed, value);
    updateAbilities();
}

float EndstonePlayer::getWalkSpeed() const
{
    return getPlayer().getAbilities().getFloat(AbilitiesIndex::WalkSpeed);
}

void EndstonePlayer::setWalkSpeed(float value) const
{
    getPlayer().getAbilities().setAbility(AbilitiesIndex::WalkSpeed, value);
    updateAbilities();
}

Scoreboard &EndstonePlayer::getScoreboard() const
{
    return server_.getPlayerBoard(*this);
}

void EndstonePlayer::setScoreboard(Scoreboard &scoreboard)
{
    server_.setPlayerBoard(*this, scoreboard);
}

void EndstonePlayer::sendTitle(std::string title, std::string subtitle) const
{
    sendTitle(std::move(title), std::move(subtitle), 10, 70, 20);
}

void EndstonePlayer::sendTitle(std::string title, std::string subtitle, int fade_in, int stay, int fade_out) const
{
    {
        auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::SetTitle);
        auto pk = std::static_pointer_cast<SetTitlePacket>(packet);
        pk->payload.type = SetTitlePacketPayload::TitleType::Title;
        pk->payload.title_text = std::move(title);
        pk->payload.fade_in_time = fade_in;
        pk->payload.stay_time = stay;
        pk->payload.fade_out_time = fade_out;
        getPlayer().sendNetworkPacket(*packet);
    }
    {
        auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::SetTitle);
        auto pk = std::static_pointer_cast<SetTitlePacket>(packet);
        pk->payload.type = SetTitlePacketPayload::TitleType::Subtitle;
        pk->payload.title_text = std::move(subtitle);
        pk->payload.fade_in_time = fade_in;
        pk->payload.stay_time = stay;
        pk->payload.fade_out_time = fade_out;
        getPlayer().sendNetworkPacket(*packet);
    }
}

void EndstonePlayer::resetTitle() const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::SetTitle);
    auto pk = std::static_pointer_cast<SetTitlePacket>(packet);
    pk->payload.type = SetTitlePacketPayload::TitleType::Reset;
    getPlayer().sendNetworkPacket(*packet);
}

void EndstonePlayer::spawnParticle(std::string name, Location location) const
{
    spawnParticle(name, location.getX(), location.getY(), location.getZ());
}

void EndstonePlayer::spawnParticle(std::string name, float x, float y, float z) const
{
    spawnParticle(name, x, y, z, std::nullopt);
}

void EndstonePlayer::spawnParticle(std::string name, Location location,
                                   std::optional<std::string> molang_variables_json) const
{
    spawnParticle(name, location.getX(), location.getY(), location.getZ(), molang_variables_json);
}

void EndstonePlayer::spawnParticle(std::string name, float x, float y, float z,
                                   std::optional<std::string> molang_variables_json) const
{
    BinaryStream stream;
    stream.writeByte(static_cast<int>(getDimension().getType()), "Dimension Id", nullptr);
    stream.writeVarInt64(-1, "Actor Unique ID", nullptr);  // -1 = self
    stream.writeFloat(x, "X", nullptr);
    stream.writeFloat(y, "Y", nullptr);
    stream.writeFloat(z, "Z", nullptr);
    stream.writeString(name, "Effect Name",
                       "Should be an effect that exists on the client. No-op if the effect doesn't exist.");
    stream.writeBool(molang_variables_json.has_value(), "Has Value",
                     "If true, follow with appropriate data type, otherwise nothing");
    if (molang_variables_json.has_value()) {
        stream.writeString(molang_variables_json.value(), "Serialized Variable Map", nullptr);
    }
    sendPacket(static_cast<int>(MinecraftPacketIds::SpawnParticleEffect), stream.getView());
}

std::chrono::milliseconds EndstonePlayer::getPing() const
{
    auto *peer = server_.getRakNetConnector().getPeer();
    const auto *component = getPlayer().tryGetComponent<UserEntityIdentifierComponent>();
    return std::chrono::milliseconds(peer->GetAveragePing(component->getNetworkId().guid));
}

void EndstonePlayer::updateCommands() const
{
    const auto &command_map = server_.getCommandMap();
    const auto &registry = command_map.getHandle().getRegistry();
    AvailableCommandsPacket packet = registry.serializeAvailableCommands();

    std::unordered_map<std::uint32_t, SemanticConstraint> constraints_to_remove;
    for (auto it = packet.commands.begin(); it != packet.commands.end();) {
        const auto &name = it->name;
        const auto command = command_map.getCommand(name);
        if (command && command->isRegistered() && command->testPermissionSilently(*static_cast<const Player *>(this))) {
            if (auto symbol = registry.findEnumValue(name); symbol.value() != 0) {
                auto symbol_index = static_cast<std::uint32_t>(symbol.toIndex());
                if (it->permission_level >= CommandPermissionLevel::Host) {
                    constraints_to_remove.emplace(symbol_index, SemanticConstraint::RequiresHostPermissions);
                }
                else if (it->permission_level > CommandPermissionLevel::Any) {
                    constraints_to_remove.emplace(symbol_index, SemanticConstraint::RequiresElevatedPermissions);
                }
            }
            it->permission_level = CommandPermissionLevel::Any;
            ++it;
        }
        else {
            it = packet.commands.erase(it);
        }
    }

    // Remove semantic constraints
    const auto enum_index = registry.findEnum("CommandName").toIndex();
    for (auto &data : packet.constraints) {
        if (constraints_to_remove.contains(data.enum_value_symbol) && data.enum_symbol == enum_index) {
            auto constraint = constraints_to_remove.at(data.enum_value_symbol);
            std::erase(data.constraints, registry.semantic_constraint_lookup_.at(constraint));
        }
    }
    getPlayer().sendNetworkPacket(packet);
}

bool EndstonePlayer::performCommand(std::string command) const
{
    return server_.dispatchCommand(*asPlayer(), command);
}

bool EndstonePlayer::isSneaking() const
{
    return getPlayer().isSneaking();
}

void EndstonePlayer::setSneaking(bool sneak)
{
    getPlayer().setSneaking(sneak);
}

bool EndstonePlayer::isSprinting() const
{
    return getPlayer().isSprinting();
}

void EndstonePlayer::setSprinting(bool sprinting)
{
    getPlayer().setSprinting(sprinting);
}

void EndstonePlayer::playSound(Location location, std::string sound, float volume, float pitch)
{
    const auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::PlaySound);
    const auto pk = std::static_pointer_cast<PlaySoundPacket>(packet);
    pk->name = sound;
    pk->pos = {static_cast<int>(location.getX()), static_cast<int>(location.getY()), static_cast<int>(location.getZ())};
    pk->volume = volume;
    pk->pitch = pitch;
    getPlayer().sendNetworkPacket(*packet);
}

void EndstonePlayer::stopSound(std::string sound)
{
    const auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::StopSound);
    const auto pk = std::static_pointer_cast<StopSoundPacket>(packet);
    pk->name = sound;
    getPlayer().sendNetworkPacket(*packet);
}

void EndstonePlayer::stopAllSounds()
{
    const auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::StopSound);
    const auto pk = std::static_pointer_cast<StopSoundPacket>(packet);
    pk->stop_all = true;
    getPlayer().sendNetworkPacket(*packet);
}

GameMode EndstonePlayer::getGameMode() const
{
    return EndstoneGameMode::fromMinecraft(getPlayer().getPlayerGameType());
}

void EndstonePlayer::setGameMode(GameMode mode)
{
    getPlayer().setPlayerGameType(EndstoneGameMode::toMinecraft(mode));
}

PlayerInventory &EndstonePlayer::getInventory() const
{
    return *inventory_;
}

Inventory &EndstonePlayer::getEnderChest() const
{
    return *ender_chest_;
}

std::string EndstonePlayer::getLocale() const
{
    return locale_;
}

std::string EndstonePlayer::getDeviceOS() const
{
    return device_os_;
}

std::string EndstonePlayer::getDeviceId() const
{
    return device_id_;
}

std::string EndstonePlayer::getGameVersion() const
{
    return game_version_;
}

Skin EndstonePlayer::getSkin() const
{
    return EndstoneSkin::fromMinecraft(getPlayer().getSkin());
}

void EndstonePlayer::transfer(std::string host, int port) const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::Transfer);
    auto pk = std::static_pointer_cast<TransferPacket>(packet);
    pk->payload.destination = std::move(host);
    pk->payload.destination_port = port;
    getPlayer().sendNetworkPacket(*packet);
}

void EndstonePlayer::sendForm(FormVariant form)
{
    if (isDead()) {
        return;
    }
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::ShowModalForm);
    std::shared_ptr<ModalFormRequestPacket> pk = std::static_pointer_cast<ModalFormRequestPacket>(packet);
    pk->payload.form_id = ++form_ids_;
    pk->payload.form_json = std::visit(overloaded{[](auto &&arg) {
                                           return FormCodec::toJson(arg);
                                       }},
                                       form)
                                .dump();
    forms_.emplace(pk->payload.form_id, std::move(form));
    getPlayer().sendNetworkPacket(*packet);
}

void EndstonePlayer::closeForm()
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::ClientboundCloseScreen);
    getPlayer().sendNetworkPacket(*packet);
    forms_.clear();
}

void EndstonePlayer::onFormClose(std::uint32_t form_id, PlayerFormCloseReason /*reason*/)
{
    auto it = forms_.find(form_id);
    if (it == forms_.end()) {
        return;  // Could be a form created via the script api, do nothing
    }

    auto form_variant = std::move(it->second);
    forms_.erase(it);

    if (!isDead()) {
        try {
            std::visit(overloaded{[this](auto &&form) {
                           auto callback = form.getOnClose();
                           if (callback) {
                               callback(this);
                           }
                       }},
                       form_variant);
        }
        catch (std::exception &e) {
            getServer().getLogger().error("Error occurred when calling a on close callback of a form: {}", e.what());
        }
    }
}

void EndstonePlayer::onFormResponse(std::uint32_t form_id, const nlohmann::json &json)
{
    auto it = forms_.find(form_id);
    if (it == forms_.end()) {
        return;  // Could be a form created via the script api, do nothing
    }

    auto form_variant = std::move(it->second);
    forms_.erase(it);

    if (!isDead()) {
        try {
            std::visit(overloaded{
                           [&](const MessageForm &form) {
                               if (const auto callback = form.getOnSubmit()) {
                                   callback(this, json.get<bool>() ? 0 : 1);
                               }
                           },
                           [&](const ActionForm &form) {
                               const int selection = json.get<int>();
                               if (const auto callback = form.getOnSubmit()) {
                                   callback(this, selection);
                               }
                               int index = 0;
                               for (const auto &controls = form.getControls(); const auto &control : controls) {
                                   if (std::holds_alternative<Button>(control)) {
                                       if (index == selection) {
                                           if (const auto on_click = std::get<Button>(control).getOnClick()) {
                                               on_click(this);
                                           }
                                           break;
                                       }
                                       ++index;
                                   }
                               }
                           },
                           [&](const ModalForm &form) {
                               if (auto callback = form.getOnSubmit()) {
                                   callback(this, json.dump());
                               }
                           },
                       },
                       form_variant);
        }
        catch (std::exception &e) {
            getServer().getLogger().error("Error occurred when calling a on submit callback of a form: {}", e.what());
        }
    }
}

void EndstonePlayer::doFirstSpawn()
{
    if (spawned_) {
        return;
    }
    spawned_ = true;

    const auto &server = static_cast<EndstoneServer &>(getServer());
    Message join_message = Translatable(ColorFormat::Yellow + "%multiplayer.player.joined", {getName()});
    PlayerJoinEvent e{*this, join_message};
    server.getPluginManager().callEvent(e);
    join_message = e.getJoinMessage().value_or("");
    if (server.getServer().getServerTextSettings()->getEnabledServerTextEvents().test(
            static_cast<std::underlying_type_t<ServerTextEvent>>(ServerTextEvent::PlayerConnection)) &&
        (!std::holds_alternative<std::string>(join_message) || !std::get<std::string>(join_message).empty())) {
        server.broadcastMessage(join_message);
    }

    recalculatePermissions();
    updateCommands();
}

void EndstonePlayer::initFromConnectionRequest(
    std::variant<const ::ConnectionRequest *, const ::SubClientConnectionRequest *> request)
{
    std::visit(
        [&](auto &&req) {
            if (auto locale = req->getLanguageCode(); !locale.empty()) {
                locale_ = locale;
            }

            // https://github.com/GeyserMC/Geyser/blob/master/common/src/main/java/org/geysermc/floodgate/util/DeviceOs.java
            auto platform = req->getDeviceOS();
            switch (platform) {
            case BuildPlatform::Google:
                device_os_ = "Android";
                break;
            case BuildPlatform::OSX:
                device_os_ = "macOS";
                break;
            case BuildPlatform::GearVR_Deprecated:
                device_os_ = "Gear VR";
                break;
            case BuildPlatform::UWP:
                device_os_ = "Windows";
                break;
            case BuildPlatform::Win32:
                device_os_ = "Windows x86";
                break;
            case BuildPlatform::tvOS_Deprecated:
                device_os_ = "Apple TV";
                break;
            case BuildPlatform::Sony:
                device_os_ = "PlayStation";
                break;
            case BuildPlatform::Nx:
                device_os_ = "Switch";
                break;
            case BuildPlatform::WindowsPhone_Deprecated:
                device_os_ = "Windows Phone";
                break;
            default:
                device_os_ = magic_enum::enum_name(platform);
                break;
            }

            if (auto device_id = req->getDeviceId(); !device_id.empty()) {
                device_id_ = device_id;
            }

            if (auto game_version = req->getGameVersionString(); !game_version.empty()) {
                game_version_ = game_version;
            }
            else {
                game_version_ = server_.getMinecraftVersion();
            }

            {
                auto skin_id = req->getSkinId();
                auto skin_height = req->getSkinImageHeight();
                auto skin_width = req->getSkinImageWidth();
                auto skin_image = Image::fromArray(Image::Type::RGBA, skin_width, skin_height, req->getSkinData());
                if (!skin_image) {
                    server_.getLogger().error("Player {} has an invalid skin: {}", getName(), skin_image.error());
                    return;
                }

                auto cape_id = req->getCapeId();
                auto cape_height = req->getCapeImageHeight();
                auto cape_width = req->getCapeImageWidth();
                auto cape_image = Image::fromArray(Image::Type::RGBA, skin_width, skin_height, req->getCapeData());
                if (cape_id.empty() || !cape_image) {
                    skin_ = std::make_unique<Skin>(skin_id, skin_image.value());
                }
                else {
                    skin_ = std::make_unique<Skin>(skin_id, skin_image.value(), cape_id, cape_image.value());
                }
            }
        },
        request);
}

void EndstonePlayer::disconnect()
{
    server_.removePlayerBoard(*this);
}

void EndstonePlayer::updateAbilities() const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::UpdateAbilitiesPacket);
    std::shared_ptr<UpdateAbilitiesPacket> pk = std::static_pointer_cast<UpdateAbilitiesPacket>(packet);
    pk->data = {getPlayer().getOrCreateUniqueID(), getPlayer().getAbilities()};
    getPlayer().sendNetworkPacket(*packet);
}

void EndstonePlayer::checkOpStatus()
{
    if (last_op_status_ != isOp()) {
        recalculatePermissions();
        updateCommands();
        last_op_status_ = isOp();
    }
}

::Player &EndstonePlayer::getPlayer() const
{
    return getHandle<::Player>();
}

}  // namespace endstone::core
