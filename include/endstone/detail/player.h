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

#pragma once

#include "endstone/detail/actor/human.h"
#include "endstone/player.h"

class Player;
class ServerNetworkHandler;

namespace endstone::detail {

class EndstonePlayer : public EndstoneHumanActor, public endstone::Player {
public:
    explicit EndstonePlayer(EndstoneServer &server, ::Player &player);
    ~EndstonePlayer() override;

    // CommandSender
    void sendMessage(const std::string &message) const override;
    void sendErrorMessage(const std::string &message) const override;
    [[nodiscard]] Server &getServer() const override;
    [[nodiscard]] std::string getName() const override;

    // Permissible
    [[nodiscard]] bool isPermissionSet(std::string name) const override;
    [[nodiscard]] bool isPermissionSet(const Permission &perm) const override;
    [[nodiscard]] bool hasPermission(std::string name) const override;
    [[nodiscard]] bool hasPermission(const Permission &perm) const override;
    PermissionAttachment *addAttachment(Plugin &plugin, const std::string &name, bool value) override;
    PermissionAttachment *addAttachment(Plugin &plugin) override;
    bool removeAttachment(PermissionAttachment &attachment) override;
    void recalculatePermissions() override;
    [[nodiscard]] std::unordered_set<PermissionAttachmentInfo *> getEffectivePermissions() const override;
    [[nodiscard]] bool isOp() const override;
    void setOp(bool value) override;

    // Actor
    [[nodiscard]] std::uint64_t getRuntimeId() const override;
    Location getLocation() const override;
    std::shared_ptr<Level> getLevel() const override;

    // Player
    [[nodiscard]] UUID getUniqueId() const override;
    [[nodiscard]] const SocketAddress &getAddress() const override;
    void sendRawMessage(std::string message) const override;
    void sendPopup(std::string message) const override;
    void sendTip(std::string message) const override;
    void kick(std::string message) const override;
    [[nodiscard]] std::chrono::milliseconds getPing() const override;
    void updateCommands() const override;

    void disconnect();

private:
    friend class ::ServerNetworkHandler;

    ::Player &player_;
    ::ServerNetworkHandler *network_handler_;
    UUID uuid_;
    SocketAddress address_;
};

}  // namespace endstone::detail
