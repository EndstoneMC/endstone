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

#include "bedrock/certificates/extended_certificate.h"
#include "bedrock/deps/raknet/rak_peer_interface.h"
#include "bedrock/entity/components/user_entity_identifier_component.h"
#include "bedrock/network/packet.h"
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
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/level/level.h"
#include "endstone/color_format.h"
#include "endstone/core/base64.h"
#include "endstone/core/form/form_codec.h"
#include "endstone/core/game_mode.h"
#include "endstone/core/inventory/player_inventory.h"
#include "endstone/core/network/packet_adapter.h"
#include "endstone/core/permissions/permissible.h"
#include "endstone/core/server.h"
#include "endstone/core/util/error.h"
#include "endstone/core/util/uuid.h"
#include "endstone/form/action_form.h"
#include "endstone/form/message_form.h"

namespace endstone::core {

EndstonePlayer::EndstonePlayer(EndstoneServer &server, ::Player &player)
    : EndstoneMob(server, player), perm_(PermissibleBase::create(static_cast<Player *>(this))),
      inventory_(std::make_unique<EndstonePlayerInventory>(player))
{
    auto *component = player.tryGetComponent<UserEntityIdentifierComponent>();
    if (!component) {
        throw std::runtime_error("UserEntityIdentifierComponent is not valid");
    }
    uuid_ = EndstoneUUID::fromMinecraft(component->client_uuid);
    xuid_ = ExtendedCertificate::getXuid(*component->certificate, false);

    switch (component->network_id.getType()) {
    case NetworkIdentifier::Type::RakNet: {
        auto *peer = entt::locator<RakNet::RakPeerInterface *>::value();
        auto addr = peer->GetSystemAddressFromGuid(component->network_id.guid);
        component->network_id.sock.sa_stor = addr.address.sa_stor;
    }
    case NetworkIdentifier::Type::Address:
    case NetworkIdentifier::Type::Address6: {
        address_ = {component->network_id.getAddress(), component->network_id.getPort()};
        break;
    }
    case NetworkIdentifier::Type::NetherNet:
    case NetworkIdentifier::Type::Invalid:
    default:
        break;
    }
}

EndstonePlayer::~EndstonePlayer() = default;

Player *EndstonePlayer::asPlayer() const
{
    return Player::asPlayer();
}

void EndstonePlayer::sendMessage(const Message &message) const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::Text);
    auto pk = std::static_pointer_cast<TextPacket>(packet);
    std::visit(overloaded{[&pk](const std::string &msg) {
                              pk->type = TextPacketType::Raw;
                              pk->message = msg;
                          },
                          [&pk](const Translatable &msg) {
                              pk->type = TextPacketType::Translate;
                              pk->message = msg.getText();
                              pk->params = msg.getParameters();
                              pk->localize = true;
                          }},
               message);
    getPlayer().sendNetworkPacket(*packet);
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

Server &EndstonePlayer::getServer() const
{
    return EndstoneMob::getServer();
}

std::string EndstonePlayer::getName() const
{
    return EndstoneMob::getName();
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

Result<PermissionAttachment *> EndstonePlayer::addAttachment(Plugin &plugin, const std::string &name, bool value)
{
    return perm_->addAttachment(plugin, name, value);
}

Result<PermissionAttachment *> EndstonePlayer::addAttachment(Plugin &plugin)
{
    return perm_->addAttachment(plugin);
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

bool EndstonePlayer::isOp() const
{
    return getPlayer().getCommandPermissionLevel() > CommandPermissionLevel::Any;
}

void EndstonePlayer::setOp(bool value)
{
    if (value == isOp()) {
        return;
    }

    getPlayer().setPermissions(value ? CommandPermissionLevel::Any : CommandPermissionLevel::Admin);
}

std::string EndstonePlayer::getType() const
{
    return EndstoneMob::getType();
}

std::uint64_t EndstonePlayer::getRuntimeId() const
{
    return EndstoneMob::getRuntimeId();
}

Location EndstonePlayer::getLocation() const
{
    return EndstoneMob::getLocation();
}

Vector<float> EndstonePlayer::getVelocity() const
{
    return EndstoneMob::getVelocity();
}

bool EndstonePlayer::isOnGround() const
{
    return EndstoneMob::isOnGround();
}

bool EndstonePlayer::isInWater() const
{
    return EndstoneMob::isInWater();
}

bool EndstonePlayer::isInLava() const
{
    return EndstoneMob::isInLava();
}

Level &EndstonePlayer::getLevel() const
{
    return EndstoneMob::getLevel();
}

Dimension &EndstonePlayer::getDimension() const
{
    return EndstoneMob::getDimension();
}

void EndstonePlayer::setRotation(float yaw, float pitch)
{
    EndstoneMob::setRotation(yaw, pitch);
}

void EndstonePlayer::teleport(Location location)
{
    EndstoneMob::teleport(location);
}

void EndstonePlayer::teleport(Actor &target)
{
    EndstoneMob::teleport(target);
}

std::int64_t EndstonePlayer::getId() const
{
    return EndstoneMob::getId();
}

void EndstonePlayer::remove()
{
    getServer().getLogger().error("Cannot remove player {}, use Player::kick instead.", getName());
}

bool EndstonePlayer::isValid() const
{
    return EndstoneMob::isValid();
}

bool EndstonePlayer::isDead() const
{
    return EndstoneMob::isDead();
}

int EndstonePlayer::getHealth() const
{
    return EndstoneMob::getHealth();
}

Result<void> EndstonePlayer::setHealth(int health) const
{
    return EndstoneMob::setHealth(health);
}

int EndstonePlayer::getMaxHealth() const
{
    return EndstoneMob::getMaxHealth();
}

std::vector<std::string> EndstonePlayer::getScoreboardTags() const
{
    return EndstoneMob::getScoreboardTags();
}

bool EndstonePlayer::addScoreboardTag(std::string tag) const
{
    return EndstoneMob::addScoreboardTag(tag);
}

bool EndstonePlayer::removeScoreboardTag(std::string tag) const
{
    return EndstoneMob::removeScoreboardTag(tag);
}

bool EndstonePlayer::isNameTagVisible() const
{
    return EndstoneMob::isNameTagVisible();
}

void EndstonePlayer::setNameTagVisible(bool visible)
{
    EndstoneMob::setNameTagVisible(visible);
}

bool EndstonePlayer::isNameTagAlwaysVisible() const
{
    return EndstoneMob::isNameTagAlwaysVisible();
}

void EndstonePlayer::setNameTagAlwaysVisible(bool visible)
{
    EndstoneMob::setNameTagAlwaysVisible(visible);
}

std::string EndstonePlayer::getNameTag() const
{
    return EndstoneMob::getNameTag();
}

void EndstonePlayer::setNameTag(std::string name)
{
    EndstoneMob::setNameTag(name);
}

std::string EndstonePlayer::getScoreTag() const
{
    return EndstoneMob::getScoreTag();
}

void EndstonePlayer::setScoreTag(std::string score)
{
    EndstoneMob::setScoreTag(score);
}

bool EndstonePlayer::isGliding() const
{
    return EndstoneMob::isGliding();
}

UUID EndstonePlayer::getUniqueId() const
{
    return uuid_;
}

std::string EndstonePlayer::getXuid() const
{
    return xuid_;
}

const SocketAddress &EndstonePlayer::getAddress() const
{
    return address_;
}

void EndstonePlayer::sendPopup(std::string message) const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::Text);
    auto pk = std::static_pointer_cast<TextPacket>(packet);
    pk->type = TextPacketType::Popup;
    pk->message = std::move(message);
    getPlayer().sendNetworkPacket(*packet);
}

void EndstonePlayer::sendTip(std::string message) const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::Text);
    auto pk = std::static_pointer_cast<TextPacket>(packet);
    pk->type = TextPacketType::Tip;
    pk->message = std::move(message);
    getPlayer().sendNetworkPacket(*packet);
}

void EndstonePlayer::sendToast(std::string title, std::string content) const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::ToastRequest);
    auto pk = std::static_pointer_cast<ToastRequestPacket>(packet);
    pk->title = std::move(title);
    pk->content = std::move(content);
    getPlayer().sendNetworkPacket(*packet);
}

void EndstonePlayer::kick(std::string message) const
{
    auto *component = getPlayer().tryGetComponent<UserEntityIdentifierComponent>();
    server_.getServer().getMinecraft()->getServerNetworkHandler()->disconnectClient(
        component->network_id, component->client_sub_id, Connection::DisconnectFailReason::Kicked, message,
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
    if (progress < 0.0 || progress > 1.0) {
        return nonstd::make_unexpected(make_error("Experience progress must be between 0.0 and 1.0 ({})", progress));
    }
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
    if (level < 0) {
        return nonstd::make_unexpected(make_error("Experience level must not be negative ({})", level));
    }
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
        return nonstd::make_unexpected(make_error("Player {} is not allowed to fly.", getName()));
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
        pk->type = SetTitlePacket::TitleType::Title;
        pk->title_text = std::move(title);
        pk->fade_in_time = fade_in;
        pk->stay_time = stay;
        pk->fade_out_time = fade_out;
        getPlayer().sendNetworkPacket(*packet);
    }
    {
        auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::SetTitle);
        auto pk = std::static_pointer_cast<SetTitlePacket>(packet);
        pk->type = SetTitlePacket::TitleType::Subtitle;
        pk->title_text = std::move(subtitle);
        pk->fade_in_time = fade_in;
        pk->stay_time = stay;
        pk->fade_out_time = fade_out;
        getPlayer().sendNetworkPacket(*packet);
    }
}

void EndstonePlayer::resetTitle() const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::SetTitle);
    auto pk = std::static_pointer_cast<SetTitlePacket>(packet);
    pk->type = SetTitlePacket::TitleType::Reset;
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
    SpawnParticleEffectPacket pk;
    pk.dimension_id = static_cast<int>(getDimension().getType());
    pk.effect_name = name;
    pk.position = {x, y, z};
    pk.molang_variables_json = molang_variables_json;
    sendPacket(pk);
}

std::chrono::milliseconds EndstonePlayer::getPing() const
{
    auto *peer = entt::locator<RakNet::RakPeerInterface *>::value();
    auto *component = getPlayer().tryGetComponent<UserEntityIdentifierComponent>();
    return std::chrono::milliseconds(peer->GetAveragePing(component->network_id.guid));
}

void EndstonePlayer::updateCommands() const
{
    auto &registry = server_.getServer().getMinecraft()->getCommands().getRegistry();
    AvailableCommandsPacket packet = registry.serializeAvailableCommands();

    auto &command_map = server_.getCommandMap();
    for (auto it = packet.commands.begin(); it != packet.commands.end();) {
        auto &name = it->name;
        auto *command = command_map.getCommand(name);
        if (command && command->isRegistered() && command->testPermissionSilently(*static_cast<const Player *>(this)) &&
            it->permission_level <= getPlayer().getCommandPermissionLevel()) {
            ++it;
            continue;
        }
        it = packet.commands.erase(it);
    }

    getPlayer().sendNetworkPacket(packet);
}

bool EndstonePlayer::performCommand(std::string command) const
{
    return server_.dispatchCommand(*Player::asPlayer(), command);
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
    pk->pos = {location.getX(), location.getY(), location.getZ()};
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

const Skin &EndstonePlayer::getSkin() const
{
    return skin_;
}

void EndstonePlayer::transfer(std::string host, int port) const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::Transfer);
    auto pk = std::static_pointer_cast<TransferPacket>(packet);
    pk->address = std::move(host);
    pk->port = port;
    getPlayer().sendNetworkPacket(*packet);
}

void EndstonePlayer::sendForm(FormVariant form)
{
    if (isDead()) {
        return;
    }
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::ShowModalForm);
    std::shared_ptr<ModalFormRequestPacket> pk = std::static_pointer_cast<ModalFormRequestPacket>(packet);
    pk->form_id = ++form_ids_;
    pk->form_json = std::visit(overloaded{[](auto &&arg) {
                                   return FormCodec::toJson(arg);
                               }},
                               form)
                        .dump();
    forms_.emplace(pk->form_id, std::move(form));
    getPlayer().sendNetworkPacket(*packet);
}

void EndstonePlayer::closeForm()
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::ClientboundCloseScreen);
    getPlayer().sendNetworkPacket(*packet);
    forms_.clear();
}

void EndstonePlayer::sendPacket(Packet &packet) const
{
    PacketAdapter pk{packet};
    getPlayer().sendNetworkPacket(pk);
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
                               if (auto callback = form.getOnSubmit()) {
                                   callback(this, json.get<bool>() ? 0 : 1);
                               }
                           },
                           [&](const ActionForm &form) {
                               int selection = json.get<int>();
                               if (auto callback = form.getOnSubmit()) {
                                   callback(this, selection);
                               }
                               const auto &buttons = form.getButtons();
                               if (selection >= 0 && selection < buttons.size()) {
                                   const auto &button = buttons[selection];
                                   if (auto on_click = button.getOnClick()) {
                                       on_click(this);
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

void EndstonePlayer::initFromConnectionRequest(
    std::variant<const ::ConnectionRequest *, const ::SubClientConnectionRequest *> request)
{
    std::visit(
        [&](auto &&req) {
            if (auto locale = req->getData("LanguageCode").asString(); !locale.empty()) {
                locale_ = locale;
            }

            // https://github.com/GeyserMC/Geyser/blob/master/common/src/main/java/org/geysermc/floodgate/util/DeviceOs.java
            if (auto device_os = req->getData("DeviceOS").asInt(); device_os > 0) {
                auto platform = magic_enum::enum_cast<BuildPlatform>(device_os).value_or(BuildPlatform::Unknown);
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
            }

            if (auto device_id = req->getData("DeviceId").asString(); !device_id.empty()) {
                device_id_ = device_id;
            }

            if (auto game_version = req->getData("GameVersion").asString(); !game_version.empty()) {
                game_version_ = game_version;
            }
            else {
                game_version_ = server_.getMinecraftVersion();
            }

            {
                auto skin_id = req->getData("SkinId").asString();
                auto skin_height = req->getData("SkinImageHeight").asInt();
                auto skin_width = req->getData("SkinImageWidth").asInt();
                auto skin_data = base64_decode(req->getData("SkinData").asString()).value_or("");
                auto cape_id = req->getData("CapeId").asString();
                auto cape_height = req->getData("CapeImageHeight").asInt();
                auto cape_width = req->getData("CapeImageWidth").asInt();
                auto cape_data = base64_decode(req->getData("CapeData").asString()).value_or("");
                skin_ = {skin_id, Skin::ImageData{skin_height, skin_width, skin_data}, cape_id,
                         Skin::ImageData{cape_height, cape_width, cape_data}};
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

std::shared_ptr<EndstonePlayer> EndstonePlayer::create(EndstoneServer &server, ::Player &player)
{
    return PermissibleFactory::create<EndstonePlayer>(server, player);
}

::Player &EndstonePlayer::getPlayer() const
{
    return getHandle<::Player>();
}

}  // namespace endstone::core
