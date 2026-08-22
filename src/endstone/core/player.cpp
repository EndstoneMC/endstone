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

#include <RakPeerInterface.h>

#include <unordered_map>
#include <unordered_set>

#include <magic_enum/magic_enum.hpp>
#include <nlohmann/json.hpp>

#include "bedrock/entity/components/user_entity_identifier_component.h"
#include "bedrock/network/packet.h"
#include "bedrock/network/packet/clientbound_map_item_data_packet.h"
#include "bedrock/network/packet/modal_form_request_packet.h"
#include "bedrock/network/packet/play_sound_packet.h"
#include "bedrock/network/packet/set_title_packet.h"
#include "bedrock/network/packet/stop_sound_packet.h"
#include "bedrock/network/packet/text_packet.h"
#include "bedrock/network/packet/toast_request_packet.h"
#include "bedrock/network/packet/transfer_packet.h"
#include "bedrock/network/packet/update_abilities_packet.h"
#include "bedrock/network/server_network_handler.h"
#include "bedrock/platform/build_platform.h"
#include "bedrock/server/server_instance.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/level/dimension/vanilla_dimensions.h"
#include "bedrock/world/level/level.h"
#include "endstone/color_format.h"
#include "endstone/core/base64.h"
#include "endstone/core/entity/components/flag_components.h"
#include "endstone/core/form/form_codec.h"
#include "endstone/core/game_mode.h"
#include "endstone/core/inventory/item_stack.h"
#include "endstone/core/inventory/player_inventory.h"
#include "endstone/core/level/dimension.h"
#include "endstone/core/map/map_view.h"
#include "endstone/core/message.h"
#include "endstone/core/network/data_packet.h"
#include "endstone/core/server.h"
#include "endstone/core/skin.h"
#include "endstone/core/util/socket_address.h"
#include "endstone/core/util/uuid.h"
#include "endstone/event/player/player_join_event.h"
#include "endstone/form/action_form.h"
#include "endstone/form/message_form.h"

namespace endstone::core {

EndstonePlayer::EndstonePlayer(EndstoneServer &server, ::Player &player)
    : EndstoneMobBase(server, player), perm_(std::make_shared<PermissibleBase>(static_cast<Player *>(this))),
      inventory_(std::make_unique<EndstonePlayerInventory>(*this)),
      ender_chest_(std::make_unique<EndstoneInventory>(
          [this]() -> ::Container & { return *getHandle().getEnderChestContainer(); })),
      address_(EndstoneSocketAddress::fromNetworkIdentifier(
          player.getPersistentComponent<UserEntityIdentifierComponent>()->getNetworkId()))
{
    last_op_status_ = EndstonePlayer::isOp();
}

PermissionLevel EndstonePlayer::getPermissionLevel() const
{
    return isOp() ? PermissionLevel::Operator : PermissionLevel::Default;
}

bool EndstonePlayer::isPermissionSet(std::string name) const
{
    return perm_->isPermissionSet(name);
}

bool EndstonePlayer::isPermissionSet(const NotNull<Permission> &perm) const
{
    return perm_->isPermissionSet(perm);
}

bool EndstonePlayer::hasPermission(std::string name) const
{
    return perm_->hasPermission(name);
}

bool EndstonePlayer::hasPermission(const NotNull<Permission> &perm) const
{
    return perm_->hasPermission(perm);
}

NotNull<PermissionAttachment> EndstonePlayer::addAttachment(Plugin &plugin, const std::string &name, bool value)
{
    return perm_->addAttachment(plugin, name, value);
}

NotNull<PermissionAttachment> EndstonePlayer::addAttachment(Plugin &plugin)
{
    return perm_->addAttachment(plugin);
}

bool EndstonePlayer::removeAttachment(const NotNull<PermissionAttachment> &attachment)
{
    return perm_->removeAttachment(attachment);
}

void EndstonePlayer::recalculatePermissions()
{
    perm_->recalculatePermissions();
}

std::unordered_set<NotNull<PermissionAttachmentInfo>> EndstonePlayer::getEffectivePermissions() const
{
    return perm_->getEffectivePermissions();
}

void EndstonePlayer::sendMessage(const Message &message) const
{
    Preconditions::checkArgument(!std::visit([](const auto &msg) { return msg.empty(); }, message),
                                 "Message must not be empty");
    if (!tryGetHandle()) {
        return;
    }

    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::Text);
    auto &pk = static_cast<TextPacket &>(*packet);
    std::visit(overloaded{[&](const std::string &msg) {
                              pk.payload = {.body = TextPacketPayload::MessageOnly{TextPacketType::Raw, msg}};
                          },
                          [&](const Translatable &msg) {
                              nlohmann::json entry;
                              entry["translate"] = msg.getText();
                              if (!msg.getParameters().empty()) {
                                  nlohmann::json with = nlohmann::json::array();
                                  for (const auto &param : msg.getParameters()) {
                                      with.push_back({{"text", param}});
                                  }
                                  entry["with"] = {{"rawtext", with}};
                              }
                              nlohmann::json rawtext = {{"rawtext", nlohmann::json::array({entry})}};
                              pk.payload = {
                                  .body = TextPacketPayload::MessageOnly{TextPacketType::TextObject, rawtext.dump()}};
                          }},
               message);
    getHandle().sendNetworkPacket(*packet);
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
    return getHandle().getName();
}

void EndstonePlayer::remove()
{
    getServer().getLogger().error("Cannot remove player {}, use Player::kick instead.", getName());
}

bool EndstonePlayer::teleport(const Location &location)
{
    if (getHealth() == 0 || getHandle().isRemoved()) {
        return false;
    }

    setRotation(location.getYaw(), location.getPitch());
    Vec3 to_location{location.getX(), location.getY(), location.getZ()};
    const NotNull<Dimension> location_dimension = location.getDimension();
    if (location_dimension != getDimension()) {
        auto current_location = getLocation();
        Vec3 from_location{current_location.getX(), current_location.getY(), current_location.getZ()};
        const auto from_dimension = getDimension().cast<EndstoneDimension>()->getHandle().getDimensionId();
        const auto to_dimension = location_dimension.cast<EndstoneDimension>()->getHandle().getDimensionId();
        getHandle().getLevel().requestPlayerChangeDimension(
            getHandle(),
            ChangeDimensionRequest{from_dimension, to_dimension, from_location, to_location, false, false});
    }
    else {
        getHandle().teleportTo(to_location, true, 3, 1, false);
    }
    return true;
}

UUID EndstonePlayer::getUniqueId() const
{
    const auto *component = getHandle().tryGetComponent<UserEntityIdentifierComponent>();
    return EndstoneUUID::fromMinecraft(component->getClientUUID());
}

bool EndstonePlayer::isOp() const
{
    const auto *handle = tryGetHandle();
    if (!handle) {
        return last_op_status_;
    }
    return handle->getCommandPermissionLevel() > CommandPermissionLevel::Any;
}

void EndstonePlayer::setOp(bool value)
{
    if (value == isOp()) {
        return;
    }
    getHandle().setCommandPermissions(value ? CommandPermissionLevel::Admin : CommandPermissionLevel::Any);
    recalculatePermissions();
    updateCommands();
    last_op_status_ = value;
}

std::string EndstonePlayer::getXuid() const
{
    const auto *component = getHandle().tryGetComponent<UserEntityIdentifierComponent>();
    return component->getXuid();
}

const SocketAddress &EndstonePlayer::getAddress() const
{
    return address_;
}

void EndstonePlayer::transfer(std::string host, int port) const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::Transfer);
    auto pk = std::static_pointer_cast<TransferPacket>(packet);
    pk->payload.destination = std::move(host);
    pk->payload.destination_port = port;
    getHandle().sendNetworkPacket(*packet);
}

void EndstonePlayer::kick(std::string message) const
{
    auto *component = getHandle().tryGetComponent<UserEntityIdentifierComponent>();
    server_.getServer().getMinecraft()->getServerNetworkHandler()->disconnectClientWithMessage(
        component->getNetworkId(), component->getSubClientId(), Connection::DisconnectFailReason::Kicked, message,
        std::nullopt);
}

bool EndstonePlayer::performCommand(std::string command) const
{
    return server_.dispatchCommand(self(), command);
}

std::optional<Location> EndstonePlayer::getRespawnLocation() const
{
    const auto &point = getHandle().getPlayerRespawnPoint();
    if (point.player_position == BlockPos::MIN || point.dimension == VanillaDimensions::Undefined) {
        return std::nullopt;
    }

    const auto dimension = server_.getEndstoneLevel()->getDimension(point.dimension);
    if (!dimension) {
        return std::nullopt;
    }
    return Location{dimension, point.player_position.x, point.player_position.y, point.player_position.z};
}

void EndstonePlayer::setRespawnLocation(std::optional<Location> location)
{
    if (!location) {
        getHandle().addOrRemoveComponent<InternalSpawnChangeFlagComponent>(true);
        getHandle().setRespawnPosition(BlockPos::MIN, VanillaDimensions::Undefined);
        return;
    }

    if (!location->isDimensionLoaded()) {
        return;
    }

    const auto dimension = location->getDimension();
    const auto dimension_id = static_cast<const EndstoneDimension &>(dimension.value()).getHandle().getDimensionId();
    getHandle().addOrRemoveComponent<InternalSpawnChangeFlagComponent>(true);
    getHandle().setRespawnPosition(BlockPos(location->getX(), location->getY(), location->getZ()), dimension_id);
}

bool EndstonePlayer::isSneaking() const
{
    return getHandle().isSneaking();
}

void EndstonePlayer::setSneaking(bool sneak)
{
    getHandle().setSneaking(sneak);
}

bool EndstonePlayer::isSprinting() const
{
    return getHandle().isSprinting();
}

void EndstonePlayer::setSprinting(bool sprinting)
{
    getHandle().setSprinting(sprinting);
}

void EndstonePlayer::playSound(Location location, std::string sound, float volume, float pitch)
{
    const auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::PlaySound);
    const auto pk = std::static_pointer_cast<PlaySoundPacket>(packet);
    pk->payload.name = sound;
    pk->payload.pos = {static_cast<int>(location.getX()), static_cast<int>(location.getY()),
                       static_cast<int>(location.getZ())};
    pk->payload.volume = volume;
    pk->payload.pitch = pitch;
    getHandle().sendNetworkPacket(*packet);
}

void EndstonePlayer::stopSound(std::string sound)
{
    const auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::StopSound);
    const auto pk = std::static_pointer_cast<StopSoundPacket>(packet);
    pk->payload.name = sound;
    getHandle().sendNetworkPacket(*packet);
}

void EndstonePlayer::stopAllSounds()
{
    const auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::StopSound);
    const auto pk = std::static_pointer_cast<StopSoundPacket>(packet);
    pk->payload.stop_all = true;
    getHandle().sendNetworkPacket(*packet);
}

void EndstonePlayer::giveExp(int amount)
{
    getHandle().addExperience(amount);
}

void EndstonePlayer::giveExpLevels(int amount)
{
    getHandle().addLevels(amount);
}

float EndstonePlayer::getExpProgress() const
{
    return getHandle().getLevelProgress();
}

void EndstonePlayer::setExpProgress(float progress)
{
    Preconditions::checkArgument(progress >= 0.0 && progress <= 1.0,
                                 "Experience progress must be between 0.0 and 1.0 ({})", progress);
    auto mutable_attr = getHandle().getMutableAttribute("minecraft:player.experience");
    mutable_attr->setCurrentValue(progress);
}

int EndstonePlayer::getExpLevel() const
{
    return getHandle().getPlayerLevel();
}

void EndstonePlayer::setExpLevel(int level)
{
    Preconditions::checkArgument(level >= 0, "Experience level must not be negative ({})", level);
    giveExpLevels(level - getExpLevel());
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
    return getHandle().getAbilities().getBool(AbilitiesIndex::MayFly);
}

void EndstonePlayer::setAllowFlight(bool flight)
{
    if (isFlying() && !flight) {
        getHandle().getAbilities().setAbility(AbilitiesIndex::Flying, false);
    }

    getHandle().getAbilities().setAbility(AbilitiesIndex::MayFly, flight);
    updateAbilities();
}

bool EndstonePlayer::isFlying() const
{
    return getHandle().isFlying();
}

bool EndstonePlayer::isCrawling() const
{
    return getHandle().isCrawling();
}

void EndstonePlayer::setFlying(bool value)
{
    if (!getAllowFlight()) {
        Preconditions::checkArgument(!value, "Player {} is not allowed to fly (check getAllowFlight())", getName());
    }

    getHandle().getAbilities().setAbility(AbilitiesIndex::Flying, value);
    updateAbilities();
}

float EndstonePlayer::getFlySpeed() const
{
    return getHandle().getAbilities().getFloat(AbilitiesIndex::FlySpeed);
}

void EndstonePlayer::setFlySpeed(float value) const
{
    getHandle().getAbilities().setAbility(AbilitiesIndex::FlySpeed, value);
    updateAbilities();
}

float EndstonePlayer::getWalkSpeed() const
{
    return getHandle().getAbilities().getFloat(AbilitiesIndex::WalkSpeed);
}

void EndstonePlayer::setWalkSpeed(float value) const
{
    getHandle().getAbilities().setAbility(AbilitiesIndex::WalkSpeed, value);
    updateAbilities();
    auto mutable_attr = getHandle().getMutableAttribute("minecraft:movement");
    mutable_attr->setDefaultValue(getWalkSpeed(), static_cast<int>(AttributeOperands::OPERAND_CURRENT));
}

NotNull<Scoreboard> EndstonePlayer::getScoreboard() const
{
    return server_.getPlayerBoard(self().cast<EndstonePlayer>());
}

void EndstonePlayer::setScoreboard(NotNull<Scoreboard> scoreboard)
{
    server_.setPlayerBoard(self().cast<EndstonePlayer>(), std::move(scoreboard));
}

void EndstonePlayer::sendActionBar(std::string message) const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::SetTitle);
    auto pk = std::static_pointer_cast<SetTitlePacket>(packet);
    pk->payload.type = SetTitlePacketPayload::TitleType::Actionbar;
    pk->payload.title_text = std::move(message);
    getHandle().sendNetworkPacket(*packet);
}

void EndstonePlayer::sendPopup(std::string message) const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::Text);
    auto &pk = static_cast<TextPacket &>(*packet);
    pk.payload = {.body = TextPacketPayload::MessageAndParams{TextPacketType::Popup, message, {}}};
    getHandle().sendNetworkPacket(*packet);
}

void EndstonePlayer::sendTip(std::string message) const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::Text);
    auto &pk = static_cast<TextPacket &>(*packet);
    pk.payload = {.body = TextPacketPayload::MessageOnly{TextPacketType::Tip, message}};
    getHandle().sendNetworkPacket(*packet);
}

void EndstonePlayer::sendToast(std::string title, std::string content) const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::ToastRequest);
    auto pk = std::static_pointer_cast<ToastRequestPacket>(packet);
    pk->payload.title = std::move(title);
    pk->payload.content = std::move(content);
    getHandle().sendNetworkPacket(*packet);
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
        getHandle().sendNetworkPacket(*packet);
    }
    {
        auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::SetTitle);
        auto pk = std::static_pointer_cast<SetTitlePacket>(packet);
        pk->payload.type = SetTitlePacketPayload::TitleType::Subtitle;
        pk->payload.title_text = std::move(subtitle);
        pk->payload.fade_in_time = fade_in;
        pk->payload.stay_time = stay;
        pk->payload.fade_out_time = fade_out;
        getHandle().sendNetworkPacket(*packet);
    }
}

void EndstonePlayer::resetTitle() const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::SetTitle);
    auto pk = std::static_pointer_cast<SetTitlePacket>(packet);
    pk->payload.type = SetTitlePacketPayload::TitleType::Reset;
    getHandle().sendNetworkPacket(*packet);
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
                                   std::optional<JsonObject> molang_variables) const
{
    spawnParticle(name, location.getX(), location.getY(), location.getZ(), std::move(molang_variables));
}

void EndstonePlayer::spawnParticle(std::string name, float x, float y, float z,
                                   std::optional<JsonObject> molang_variables) const
{
    BinaryStream stream;
    stream.writeByte(getHandle().getDimension().getDimensionId().value, "Dimension Id", nullptr);
    stream.writeVarInt64(-1, "Actor Unique ID", nullptr);  // -1 = self
    stream.writeFloat(x, "X", nullptr);
    stream.writeFloat(y, "Y", nullptr);
    stream.writeFloat(z, "Z", nullptr);
    stream.writeString(name, "Effect Name",
                       "Should be an effect that exists on the client. No-op if the effect doesn't exist.");
    stream.writeBool(molang_variables.has_value(), "Has Value",
                     "If true, follow with appropriate data type, otherwise nothing");
    if (molang_variables.has_value()) {
        stream.writeString(JsonValue(*molang_variables).dump(), "Serialized Variable Map", nullptr);
    }
    sendPacket(static_cast<int>(MinecraftPacketIds::SpawnParticleEffect), stream.getView());
}

std::chrono::milliseconds EndstonePlayer::getPing() const
{
    const auto *component = getHandle().tryGetComponent<UserEntityIdentifierComponent>();
    if (!component) {
        return {};
    }
    const auto *peer = server_.getServer().getNetwork().getPeerForUser(component->getNetworkId());
    if (!peer) {
        return {};
    }
    return peer->getNetworkStatus().average_ping;
}

std::string EndstonePlayer::getLocale() const
{
    return locale_;
}

void EndstonePlayer::updateCommands() const
{
    const auto &command_map = server_.getCommandMap();
    const auto &registry = command_map.getHandle().getRegistry();
    AvailableCommandsPacket packet = registry.serializeAvailableCommands();

    std::unordered_map<std::uint32_t, SemanticConstraint> constraints_to_remove;
    for (auto it = packet.payload.commands.begin(); it != packet.payload.commands.end();) {
        const auto &name = it->name;
        const auto command = command_map.getCommand(name);
        if (command && command->isRegistered() && command->testPermissionSilently(self())) {
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
            it = packet.payload.commands.erase(it);
        }
    }

    // Remove semantic constraints
    const auto enum_index = registry.findEnum("CommandName").toIndex();
    for (auto &data : packet.payload.constraints) {
        if (constraints_to_remove.contains(data.enum_value_symbol) && data.enum_symbol == enum_index) {
            auto constraint = constraints_to_remove.at(data.enum_value_symbol);
            std::erase(data.constraints, registry.semantic_constraint_lookup_.at(constraint));
        }
    }
    getHandle().sendNetworkPacket(packet);
}

PlayerInventory &EndstonePlayer::getInventory() const
{
    return *inventory_;
}

Inventory &EndstonePlayer::getEnderChest() const
{
    return *ender_chest_;
}

GameMode EndstonePlayer::getGameMode() const
{
    return EndstoneGameMode::fromMinecraft(getHandle().getPlayerGameType());
}

void EndstonePlayer::setGameMode(GameMode mode)
{
    getHandle().setPlayerGameType(EndstoneGameMode::toMinecraft(mode));
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
    return EndstoneSkin::fromMinecraft(getHandle().getSkin());
}

void EndstonePlayer::sendForm(FormVariant form)
{
    if (isDead()) {
        return;
    }
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::ShowModalForm);
    std::shared_ptr<ModalFormRequestPacket> pk = std::static_pointer_cast<ModalFormRequestPacket>(packet);
    pk->payload.form_id = ++form_ids_;
    pk->payload.form_json = std::visit(overloaded{[](auto &&arg) { return FormCodec::toJson(arg); }}, form).dump();
    forms_.emplace(pk->payload.form_id, std::move(form));
    getHandle().sendNetworkPacket(*packet);
}

void EndstonePlayer::closeForm()
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::ClientboundCloseScreen);
    getHandle().sendNetworkPacket(*packet);
    forms_.clear();
}

void EndstonePlayer::sendPacket(int packet_id, std::string_view payload) const
{
    DataPacket pk(packet_id, payload);
    getHandle().sendNetworkPacket(pk);
}

void EndstonePlayer::sendMap(MapView &map)
{
    auto &view = static_cast<EndstoneMapView &>(map);
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::MapData);
    auto &pk = static_cast<ClientboundMapItemDataPacket &>(*packet);
    pk.payload.map_id = view.map_.getMapId();
    pk.payload.scale = view.map_.getScale();
    pk.payload.start_x = 0;
    pk.payload.start_y = 0;
    pk.payload.map_origin = view.map_.getOrigin();
    pk.payload.dimension = view.map_.getDimensionId().value;
    pk.payload.width = MapConstants::MAP_SIZE;
    pk.payload.height = MapConstants::MAP_SIZE;
    pk.payload.type =
        ClientboundMapItemDataPacket::Type::TextureUpdate | ClientboundMapItemDataPacket::Type::DecorationUpdate;
    pk.payload.locked = view.map_.isLocked();

    for (const auto &[unique_id, decoration] : view.map_.getDecorations()) {
        pk.payload.unique_ids.emplace_back(unique_id);
        pk.payload.decorations.emplace_back(decoration);
    }
    pk.payload.map_pixels.resize(pk.payload.width * pk.payload.height);
    getHandle().sendNetworkPacket(*packet);
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
                               callback(self());
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
                                   callback(self(), json.get<bool>() ? 0 : 1);
                               }
                           },
                           [&](const ActionForm &form) {
                               const int selection = json.get<int>();
                               if (const auto callback = form.getOnSubmit()) {
                                   callback(self(), selection);
                               }
                               int index = 0;
                               for (const auto &controls = form.getControls(); const auto &control : controls) {
                                   if (std::holds_alternative<Button>(control)) {
                                       if (index == selection) {
                                           if (const auto on_click = std::get<Button>(control).getOnClick()) {
                                               on_click(self());
                                           }
                                           break;
                                       }
                                       ++index;
                                   }
                               }
                           },
                           [&](const ModalForm &form) {
                               if (auto callback = form.getOnSubmit()) {
                                   callback(self(), json.get<JsonArray>());
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
    PlayerJoinEvent e{self(), join_message};
    server.getPluginManager().callEvent(e);
    join_message = e.getJoinMessage().value_or("");
    if (server.isServerTextEnabled(ServerTextEvent::PlayerConnection) &&
        (!std::holds_alternative<std::string>(join_message) || !std::get<std::string>(join_message).empty())) {
        server.broadcastMessage(join_message);
    }

    recalculatePermissions();
    updateCommands();
}

void EndstonePlayer::initFromConnectionRequest(std::variant<std::reference_wrapper<const ::ConnectionRequest>,
                                                            std::reference_wrapper<const ::SubClientConnectionRequest>>
                                                   request)
{
    std::visit(
        [&](auto &&ref) {
            const auto &req = ref.get();
            if (auto locale = req.getLanguageCode(); !locale.empty()) {
                locale_ = locale;
            }

            // https://github.com/GeyserMC/Geyser/blob/master/common/src/main/java/org/geysermc/floodgate/util/DeviceOs.java
            auto platform = req.getDeviceOS();
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
            case BuildPlatform::UWP_Deprecated:
            case BuildPlatform::Win32:
                device_os_ = "Windows";
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

            if (auto device_id = req.getDeviceId(); !device_id.empty()) {
                device_id_ = device_id;
            }

            using ReqType = std::remove_cvref_t<decltype(req)>;
            if constexpr (std::is_same_v<ReqType, ::ConnectionRequest>) {
                if (auto game_version = req.getGameVersionString(); !game_version.empty()) {
                    game_version_ = game_version;
                }
                else {
                    game_version_ = server_.getMinecraftVersion();
                }
            }
            else {
                game_version_ = server_.getMinecraftVersion();
            }
        },
        request);
}

void EndstonePlayer::disconnect()
{
    server_.removePlayerBoard(self().cast<EndstonePlayer>());
    forms_.clear();  // a form callback may hold the last reference back to this player
    perm_->clearPermissions();
}

void EndstonePlayer::updateAbilities() const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::UpdateAbilitiesPacket);
    std::shared_ptr<UpdateAbilitiesPacket> pk = std::static_pointer_cast<UpdateAbilitiesPacket>(packet);
    pk->payload.data = {getHandle().getOrCreateUniqueID(), getHandle().getAbilities()};
    getHandle().sendNetworkPacket(*packet);
}

void EndstonePlayer::checkOpStatus()
{
    // TODO(fixme): replace me with client_player_permissions_publisher in PermissionsHandler
    if (last_op_status_ != isOp()) {
        recalculatePermissions();
        updateCommands();
        last_op_status_ = isOp();
    }
}
}  // namespace endstone::core
