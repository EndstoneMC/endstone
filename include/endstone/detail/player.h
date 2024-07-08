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

#include <memory>

#include "bedrock/network/packet/types/connection_request.h"
#include "bedrock/network/packet/types/sub_client_connection_request.h"
#include "bedrock/world/actor/player/build_platform.h"
#include "endstone/detail/actor/actor.h"
#include "endstone/detail/inventory/player_inventory.h"
#include "endstone/player.h"

class Player;
class ServerNetworkHandler;

namespace endstone::detail {

class EndstonePlayer : public EndstoneActor, public endstone::Player {
public:
    explicit EndstonePlayer(EndstoneServer &server, ::Player &player);
    ~EndstonePlayer() override;

    // CommandSender
    void sendMessage(const std::string &message) const override;
    void sendMessage(const Translatable &message) const override;
    void sendErrorMessage(const Translatable &message) const override;
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
    [[nodiscard]] Location getLocation() const override;
    [[nodiscard]] Vector<float> getVelocity() const override;
    [[nodiscard]] bool isOnGround() const override;
    [[nodiscard]] bool isInWater() const override;
    [[nodiscard]] bool isInLava() const override;
    [[nodiscard]] Level &getLevel() const override;
    [[nodiscard]] Dimension &getDimension() const override;

    // Player
    [[nodiscard]] UUID getUniqueId() const override;
    [[nodiscard]] const SocketAddress &getAddress() const override;
    void sendPopup(std::string message) const override;
    void sendTip(std::string message) const override;
    void kick(std::string message) const override;
    void giveExp(int amount) override;
    void giveExpLevels(int amount) override;
    [[nodiscard]] float getExpProgress() const override;
    void setExpProgress(float progress) override;
    [[nodiscard]] int getExpLevel() const override;
    void setExpLevel(int level) override;
    [[nodiscard]] int getTotalExp() const override;
    [[nodiscard]] bool getAllowFlight() const override;
    void setAllowFlight(bool flight) override;
    [[nodiscard]] bool isFlying() const override;
    void setFlying(bool value) override;
    [[nodiscard]] float getFlySpeed() const override;
    void setFlySpeed(float value) const override;
    [[nodiscard]] float getWalkSpeed() const override;
    void setWalkSpeed(float value) const override;
    [[nodiscard]] std::chrono::milliseconds getPing() const override;
    void updateCommands() const override;
    bool performCommand(std::string command) const override;  // NOLINT(*-use-nodiscard)
    [[nodiscard]] GameMode getGameMode() const override;
    void setGameMode(GameMode mode) override;
    [[nodiscard]] PlayerInventory &getInventory() const override;
    [[nodiscard]] std::string getLocale() const override;
    [[nodiscard]] std::string getDeviceOS() const override;
    [[nodiscard]] endstone::UUID getDeviceId() const override;
    const Skin &getSkin() const override;

    void initFromConnectionRequest(
        std::variant<const ::ConnectionRequest *, const ::SubClientConnectionRequest *> request);
    void disconnect();
    void updateAbilities() const;
    [[nodiscard]] ::Player &getHandle() const;

private:
    friend class ::ServerNetworkHandler;

    ::Player &player_;
    UUID uuid_;
    SocketAddress address_;
    PermissibleBase perm_;
    std::unique_ptr<EndstonePlayerInventory> inventory_;
    std::string locale_ = "en-US";
    std::string device_os_ = "unknown";
    endstone::UUID device_id_;
    Skin skin_;
};

}  // namespace endstone::detail
