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

#include "endstone/detail/player.h"

#include "bedrock/command/command_origin_data.h"
#include "bedrock/command/command_origin_loader.h"
#include "bedrock/command/command_position.h"
#include "bedrock/network/protocol/game/text_packet.h"
#include "bedrock/network/protocol/minecraft_packets.h"
#include "bedrock/network/raknet/rak_peer_interface.h"
#include "bedrock/server/network/server_network_handler.h"
#include "bedrock/world/actor/components/user_entity_identifier_component.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/level/game_type.h"
#include "bedrock/world/level/level.h"
#include "endstone/color_format.h"
#include "endstone/detail/server.h"

namespace endstone::detail {

EndstonePlayer::EndstonePlayer(EndstoneServer &server, ::Player &player)
    : EndstoneActor(server, player), player_(player), perm_(static_cast<Player *>(this))
{
    auto *component = player.tryGetComponent<UserEntityIdentifierComponent>();
    if (!component) {
        throw std::runtime_error("UserEntityIdentifierComponent is not valid");
    }
    uuid_ = component->uuid.toEndstone();

    switch (component->network_id.getType()) {
    case NetworkIdentifier::Type::RakNet: {
        auto *peer = entt::locator<RakNet::RakPeerInterface *>::value();
        auto addr = peer->GetSystemAddressFromGuid(component->network_id.raknet_guid);
        component->network_id.address.sa_stor = addr.address.sa_stor;
    }
    case NetworkIdentifier::Type::Address:
    case NetworkIdentifier::Type::Address6: {
        address_ = {component->network_id.getAddress(), component->network_id.getPort()};
        break;
    }
    case NetworkIdentifier::Type::NetherNet:
    case NetworkIdentifier::Type::Generic:
        throw std::runtime_error("Unsupported type of NetworkIdentifier");
    }

    server_.players_.emplace(uuid_, this);
}

EndstonePlayer::~EndstonePlayer()
{
    server_.players_.erase(uuid_);
}

void EndstonePlayer::sendMessage(const std::string &message) const
{
    sendRawMessage(message);
}

void EndstonePlayer::sendErrorMessage(const std::string &message) const
{
    sendMessage(ColorFormat::Red + message);
}

Server &EndstonePlayer::getServer() const
{
    return EndstoneActor::getServer();
}

std::string EndstonePlayer::getName() const
{
    return EndstoneActor::getName();
}

bool EndstonePlayer::isPermissionSet(std::string name) const
{
    return perm_.isPermissionSet(name);
}

bool EndstonePlayer::isPermissionSet(const Permission &perm) const
{
    return perm_.isPermissionSet(perm);
}

bool EndstonePlayer::hasPermission(std::string name) const
{
    return perm_.hasPermission(name);
}

bool EndstonePlayer::hasPermission(const Permission &perm) const
{
    return perm_.hasPermission(perm);
}

PermissionAttachment *EndstonePlayer::addAttachment(Plugin &plugin, const std::string &name, bool value)
{
    return perm_.addAttachment(plugin, name, value);
}

PermissionAttachment *EndstonePlayer::addAttachment(Plugin &plugin)
{
    return perm_.addAttachment(plugin);
}

bool EndstonePlayer::removeAttachment(PermissionAttachment &attachment)
{
    return perm_.removeAttachment(attachment);
}

void EndstonePlayer::recalculatePermissions()
{
    perm_.recalculatePermissions();
}

std::unordered_set<PermissionAttachmentInfo *> EndstonePlayer::getEffectivePermissions() const
{
    return perm_.getEffectivePermissions();
}

bool EndstonePlayer::isOp() const
{
    return player_.getCommandPermissionLevel() != CommandPermissionLevel::Any;
}

void EndstonePlayer::setOp(bool value)
{
    if (value == isOp()) {
        return;
    }

    player_.setPermissions(value ? CommandPermissionLevel::Any : CommandPermissionLevel::GameDirectors);
}

std::uint64_t EndstonePlayer::getRuntimeId() const
{
    return EndstoneActor::getRuntimeId();
}

Location EndstonePlayer::getLocation() const
{
    return EndstoneActor::getLocation();
}

Vector<float> EndstonePlayer::getVelocity() const
{
    return EndstoneActor::getVelocity();
}

bool EndstonePlayer::isOnGround() const
{
    return EndstoneActor::isOnGround();
}

bool EndstonePlayer::isInWater() const
{
    return EndstoneActor::isInWater();
}

bool EndstonePlayer::isInLava() const
{
    return EndstoneActor::isInLava();
}

Level &EndstonePlayer::getLevel() const
{
    return EndstoneActor::getLevel();
}

Dimension &EndstonePlayer::getDimension() const
{
    return EndstoneActor::getDimension();
}

UUID EndstonePlayer::getUniqueId() const
{
    return uuid_;
}

const SocketAddress &EndstonePlayer::getAddress() const
{
    return address_;
}

void EndstonePlayer::sendRawMessage(std::string message) const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::Text);
    std::shared_ptr<TextPacket> text_packet = std::static_pointer_cast<TextPacket>(packet);
    text_packet->type = TextPacketType::Raw;
    text_packet->message = message;
    player_.sendNetworkPacket(*packet);
}

void EndstonePlayer::sendPopup(std::string message) const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::Text);
    std::shared_ptr<TextPacket> text_packet = std::static_pointer_cast<TextPacket>(packet);
    text_packet->type = TextPacketType::Popup;
    text_packet->message = message;
    player_.sendNetworkPacket(*packet);
}

void EndstonePlayer::sendTip(std::string message) const
{
    auto packet = MinecraftPackets::createPacket(MinecraftPacketIds::Text);
    std::shared_ptr<TextPacket> text_packet = std::static_pointer_cast<TextPacket>(packet);
    text_packet->type = TextPacketType::Tip;
    text_packet->message = message;
    player_.sendNetworkPacket(*packet);
}

void EndstonePlayer::init(ServerNetworkHandler &network_handler)
{
    network_handler_ = &network_handler;
}

void EndstonePlayer::disconnect()
{
    perm_.clearPermissions();
}

void EndstonePlayer::kick(std::string message) const
{
    auto *component = player_.tryGetComponent<UserEntityIdentifierComponent>();
    network_handler_->disconnectClient(component->network_id, component->sub_client_id,
                                       Connection::DisconnectFailReason::NoReason, message, message.empty());
}

std::chrono::milliseconds EndstonePlayer::getPing() const
{
    auto *peer = entt::locator<RakNet::RakPeerInterface *>::value();
    auto *component = player_.tryGetComponent<UserEntityIdentifierComponent>();
    return std::chrono::milliseconds(peer->GetAveragePing({component->network_id.raknet_guid}));
}

void EndstonePlayer::updateCommands() const
{
    auto &registry = server_.getMinecraftCommands().getRegistry();
    AvailableCommandsPacket packet = registry.serializeAvailableCommands();

    auto &command_map = server_.getCommandMap();
    for (auto &data : packet.commands) {
        auto name = data.name;
        auto *command = command_map.getCommand(name);
        if (command && command->isRegistered() && command->testPermissionSilently(*static_cast<const Player *>(this))) {
            continue;
        }
        data.command_flag |= (CommandFlag::HiddenFromPlayer | CommandFlag::HiddenFromBlock);
    }

    player_.sendNetworkPacket(packet);
}

bool EndstonePlayer::performCommand(std::string command) const
{
    CompoundTag tag;
    {
        auto origin_type = CommandOriginType::GameDirectorEntityServer;
        auto entity_id = player_.getOrCreateUniqueID();
        CompoundTag origin;
        {
            origin.putByte("OriginType", static_cast<std::uint8_t>(origin_type));
            origin.putInt64("EntityId", entity_id.id);
        }
        CompoundTag output_receiver;
        {
            output_receiver.putByte("OriginType", static_cast<std::uint8_t>(origin_type));
            output_receiver.putInt64("EntityId", entity_id.id);
        }
        tag.putByte("OriginType", static_cast<std::uint8_t>(CommandOriginType::Virtual));
        tag.putCompound("Origin", std::move(origin));
        tag.putCompound("OutputReceiver", std::move(output_receiver));
        tag.putInt("Version", CommandVersion::CurrentVersion);
    }

    auto origin = CommandOriginLoader::load(tag, static_cast<ServerLevel &>(player_.getLevel()));
    CommandContext ctx{command, std::move(origin), CommandVersion::CurrentVersion};
    auto result = server_.getMinecraftCommands().executeCommand(ctx, true);
    return result.is_success;
}

GameMode EndstonePlayer::getGameMode() const
{
    auto game_type = player_.getPlayerGameType();
    switch (game_type) {
    case GameType::Creative:
        return GameMode::Creative;
    case GameType::Adventure:
        return GameMode::Adventure;
    case GameType::Spectator:
        return GameMode::Spectator;
    default:
        break;
    }
    return GameMode::Survival;
}

void EndstonePlayer::setGameMode(GameMode mode)
{
    switch (mode) {
    case GameMode::Survival:
        player_.setPlayerGameType(GameType::Survival);
        break;
    case GameMode::Creative:
        player_.setPlayerGameType(GameType::Creative);
        break;
    case GameMode::Adventure:
        player_.setPlayerGameType(GameType::Adventure);
        break;
    case GameMode::Spectator:
        player_.setPlayerGameType(GameType::Spectator);
        break;
    }
}

}  // namespace endstone::detail
