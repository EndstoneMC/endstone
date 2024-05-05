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

#include "bedrock/network/protocol/game/text_packet.h"
#include "bedrock/network/protocol/minecraft_packets.h"
#include "bedrock/network/raknet/rak_peer_interface.h"
#include "bedrock/server/network/server_network_handler.h"
#include "bedrock/world/actor/components/user_entity_identifier_component.h"
#include "bedrock/world/actor/player/player.h"
#include "endstone/color_format.h"
#include "endstone/detail/server.h"

namespace endstone::detail {

EndstonePlayer::EndstonePlayer(EndstoneServer &server, ::Player &player)
    : EndstoneHumanActor(server, player), player_(player)
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
    return EndstoneHumanActor::getServer();
}

std::string EndstonePlayer::getName() const
{
    return EndstoneHumanActor::getName();
}

bool EndstonePlayer::isPermissionSet(std::string name) const
{
    return EndstoneHumanActor::isPermissionSet(name);
}

bool EndstonePlayer::isPermissionSet(const Permission &perm) const
{
    return EndstoneHumanActor::isPermissionSet(perm);
}

bool EndstonePlayer::hasPermission(std::string name) const
{
    return EndstoneHumanActor::hasPermission(name);
}

bool EndstonePlayer::hasPermission(const Permission &perm) const
{
    return EndstoneHumanActor::hasPermission(perm);
}

PermissionAttachment *EndstonePlayer::addAttachment(Plugin &plugin, const std::string &name, bool value)
{
    return EndstoneHumanActor::addAttachment(plugin, name, value);
}

PermissionAttachment *EndstonePlayer::addAttachment(Plugin &plugin)
{
    return EndstoneHumanActor::addAttachment(plugin);
}

bool EndstonePlayer::removeAttachment(PermissionAttachment &attachment)
{
    return EndstoneHumanActor::removeAttachment(attachment);
}

void EndstonePlayer::recalculatePermissions()
{
    EndstoneHumanActor::recalculatePermissions();
}

std::unordered_set<PermissionAttachmentInfo *> EndstonePlayer::getEffectivePermissions() const
{
    return EndstoneHumanActor::getEffectivePermissions();
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
    return EndstoneHumanActor::getRuntimeId();
}

Location EndstonePlayer::getLocation() const
{
    return EndstoneHumanActor::getLocation();
}

std::shared_ptr<Level> EndstonePlayer::getLevel() const
{
    return EndstoneHumanActor::getLevel();
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

}  // namespace endstone::detail
