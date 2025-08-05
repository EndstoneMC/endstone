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

#include <nlohmann/json.hpp>

#include "bedrock/network/connection_request.h"
#include "bedrock/network/sub_client_connection_request.h"
#include "bedrock/world/events/player_events.h"
#include "endstone/core/actor/mob.h"
#include "endstone/core/inventory/player_inventory.h"
#include "endstone/player.h"

class Player;
class ServerNetworkHandler;

namespace endstone::core {

class EndstonePlayer : public EndstoneMob, public Player {
public:
    explicit EndstonePlayer(EndstoneServer &server, ::Player &player);

    // CommandSender
    [[nodiscard]] Player *asPlayer() const override;
    void sendMessage(const Message &message) const override;
    void sendErrorMessage(const Message &message) const override;
    [[nodiscard]] std::string getName() const override;

    // Permissible
    [[nodiscard]] PermissionLevel getPermissionLevel() const override;
    [[nodiscard]] bool isPermissionSet(std::string name) const override;
    [[nodiscard]] bool isPermissionSet(const Permission &perm) const override;
    [[nodiscard]] bool hasPermission(std::string name) const override;
    [[nodiscard]] bool hasPermission(const Permission &perm) const override;
    PermissionAttachment *addAttachment(Plugin &plugin, const std::string &name, bool value) override;
    PermissionAttachment *addAttachment(Plugin &plugin) override;
    Result<void> removeAttachment(PermissionAttachment &attachment) override;
    void recalculatePermissions() override;
    [[nodiscard]] std::unordered_set<PermissionAttachmentInfo *> getEffectivePermissions() const override;
    [[nodiscard]] Server &getServer() const override;

    // Actor
    void remove() override;

    // OfflinePlayer
    [[nodiscard]] UUID getUniqueId() const override;

    // Player
    [[nodiscard]] bool isOp() const override;
    void setOp(bool value) override;
    [[nodiscard]] std::string getXuid() const override;
    [[nodiscard]] SocketAddress getAddress() const override;
    void transfer(std::string host, int port) const override;
    void kick(std::string message) const override;
    bool performCommand(std::string command) const override;  // NOLINT(*-use-nodiscard)
    [[nodiscard]] bool isSneaking() const override;
    void setSneaking(bool sneak) override;
    [[nodiscard]] bool isSprinting() const override;
    void setSprinting(bool sprinting) override;
    void playSound(Location location, std::string sound, float volume, float pitch) override;
    void stopSound(std::string sound) override;
    void stopAllSounds() override;
    void giveExp(int amount) override;
    void giveExpLevels(int amount) override;
    [[nodiscard]] float getExpProgress() const override;
    Result<void> setExpProgress(float progress) override;
    [[nodiscard]] int getExpLevel() const override;
    Result<void> setExpLevel(int level) override;
    [[nodiscard]] int getTotalExp() const override;
    [[nodiscard]] bool getAllowFlight() const override;
    void setAllowFlight(bool flight) override;
    [[nodiscard]] bool isFlying() const override;
    Result<void> setFlying(bool value) override;
    [[nodiscard]] float getFlySpeed() const override;
    void setFlySpeed(float value) const override;
    [[nodiscard]] float getWalkSpeed() const override;
    void setWalkSpeed(float value) const override;
    [[nodiscard]] Scoreboard &getScoreboard() const override;
    void setScoreboard(Scoreboard &scoreboard) override;
    void sendPopup(std::string message) const override;
    void sendTip(std::string message) const override;
    void sendToast(std::string title, std::string content) const override;
    void sendTitle(std::string title, std::string subtitle) const override;
    void sendTitle(std::string title, std::string subtitle, int fade_in, int stay, int fade_out) const override;
    void resetTitle() const override;
    void spawnParticle(std::string name, Location location) const override;
    void spawnParticle(std::string name, float x, float y, float z) const override;
    void spawnParticle(std::string name, Location location,
                       std::optional<std::string> molang_variables_json) const override;
    void spawnParticle(std::string name, float x, float y, float z,
                       std::optional<std::string> molang_variables_json) const override;
    [[nodiscard]] std::chrono::milliseconds getPing() const override;
    void updateCommands() const override;

    [[nodiscard]] PlayerInventory &getInventory() const override;
    [[nodiscard]] Inventory &getEnderChest() const override;
    [[nodiscard]] GameMode getGameMode() const override;
    void setGameMode(GameMode mode) override;
    [[nodiscard]] std::string getLocale() const override;
    [[nodiscard]] std::string getDeviceOS() const override;
    [[nodiscard]] std::string getDeviceId() const override;
    [[nodiscard]] std::string getGameVersion() const override;
    [[nodiscard]] Skin getSkin() const override;
    void sendForm(FormVariant form) override;
    void closeForm() override;
    void sendPacket(int packet_id, std::string_view payload) const override;
    bool handlePacket(Packet &packet);
    void onFormClose(std::uint32_t form_id, PlayerFormCloseReason reason);
    void onFormResponse(std::uint32_t form_id, const nlohmann::json &json);
    void doFirstSpawn();

    void initFromConnectionRequest(
        std::variant<const ::ConnectionRequest *, const ::SubClientConnectionRequest *> request);
    void disconnect();
    void updateAbilities() const;
    void checkOpStatus();

    ::Player &getPlayer() const;

private:
    friend class ::ServerNetworkHandler;

    UUID uuid_;
    std::string xuid_;
    std::shared_ptr<PermissibleBase> perm_;
    std::unique_ptr<EndstonePlayerInventory> inventory_;
    std::unique_ptr<EndstoneInventory> ender_chest_;
    std::string locale_ = "en_US";
    std::string device_os_ = "Unknown";
    std::string device_id_;
    std::string game_version_;
    std::unique_ptr<Skin> skin_;
    std::uint32_t form_ids_ = 0xffff;  // Set to a large value to avoid collision with forms created by script api
    std::unordered_map<std::uint32_t, FormVariant> forms_;
    bool spawned_ = false;
    bool last_op_status_ = false;

public:
    // forward
    ENDSTONE_FORWARD_IMPL_ACTOR(EndstoneMob);
    ENDSTONE_FORWARD_IMPL_MOB(EndstoneMob);
    [[nodiscard]] Mob *asMob() const override
    {
        return EndstoneMob::asMob();
    }
    [[nodiscard]] Item *asItem() const override
    {
        return EndstoneMob::asItem();
    }
};

}  // namespace endstone::core
