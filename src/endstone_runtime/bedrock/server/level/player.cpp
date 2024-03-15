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

#include "bedrock/server/level/player.h"

#include "bedrock/network/game/available_commands_packet.h"
#include "endstone/detail/hook.h"
#include "endstone/detail/server.h"
#include "endstone/detail/singleton.h"

namespace {

class DummyPlayerSender : public endstone::detail::BaseCommandSender {
public:
    explicit DummyPlayerSender(CommandPermissionLevel level) : level_(level) {}
    [[nodiscard]] bool isOp() const override
    {
        return level_ != CommandPermissionLevel::Any;
    }
    void setOp(bool value) override {}
    void sendMessage(const std::string &message) const override {}
    void sendErrorMessage(const std::string &message) const override {}
    [[nodiscard]] std::string getName() const override
    {
        return "";
    }

private:
    CommandPermissionLevel level_;
};

// TODO: refactor this into endstone::Player::sendCommands or endstone::Server::syncCommands
void sendCommands(Player &player, CommandPermissionLevel level)
{
    using endstone::detail::EndstoneServer;
    using endstone::detail::Singleton;
    auto &server = Singleton<EndstoneServer>::getInstance();
    auto &registry = server.getMinecraftCommands().getRegistry();

    AvailableCommandsPacket packet = registry.serializeAvailableCommands();

    auto &command_map = server.getCommandMap();
    // TODO: use endstone::Player instead
    auto perm = DummyPlayerSender(level);
    for (auto &data : packet.commands) {
        auto name = data.name;
        auto *command = command_map.getCommand(name);
        if (command && command->isRegistered() && command->testPermissionSilently(perm)) {
            continue;
        }
        data.command_flag |= (CommandFlag::HIDDEN_FROM_PLAYER | CommandFlag::HIDDEN_FROM_BLOCK);
    }

    // TODO: use virtual function Player::sendNetworkPacket instead
    player.getPacketSender().send(packet);
}
}  // namespace

void Player::setPermissions(CommandPermissionLevel level)
{
    ENDSTONE_HOOK_CALL_ORIGINAL(&Player::setPermissions, this, level);
    sendCommands(*this, level);
}
