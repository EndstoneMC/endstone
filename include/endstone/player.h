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

#include <chrono>
#include <optional>
#include <string>
#include <string_view>
#include <variant>

#include "endstone/actor/mob.h"
#include "endstone/form/action_form.h"
#include "endstone/form/message_form.h"
#include "endstone/form/modal_form.h"
#include "endstone/game_mode.h"
#include "endstone/inventory/player_inventory.h"
#include "endstone/offline_player.h"
#include "endstone/scoreboard/scoreboard.h"
#include "endstone/skin.h"
#include "endstone/util/socket_address.h"
#include "endstone/util/uuid.h"

namespace endstone {

/**
 * @brief Represents a player.
 */
class Player : public Mob, public OfflinePlayer {
protected:
    using FormVariant = std::variant<MessageForm, ActionForm, ModalForm>;

public:
    /**
     * @brief Returns the name of this player
     *
     * @return Player name or null if we have not seen a name for this player yet
     */
    [[nodiscard]] std::string getName() const override = 0;

    /**
     * @brief Checks if this player is a server operator
     *
     * @return true if this is an operator, otherwise false
     */
    [[nodiscard]] virtual bool isOp() const = 0;

    /**
     * @brief Sets the operator status of this player
     *
     * @param value New operator value
     */
    virtual void setOp(bool value) = 0;

    /**
     * @brief Returns the Xbox User ID (XUID) of this player
     *
     * @return Player XUID
     */
    [[nodiscard]] virtual std::string getXuid() const = 0;

    /**
     * @brief Gets the socket address of this player
     *
     * @return the player's socket address
     */
    [[nodiscard]] virtual SocketAddress getAddress() const = 0;

    /**
     * @brief Transfers the player to another server
     *
     * @param host Server address to transfer the player to.
     * @param port Server port to transfer the player to
     */
    virtual void transfer(std::string host, int port) const = 0;

    /**
     * @brief Kicks player with custom kick message.
     *
     * @param message kick message
     */
    virtual void kick(std::string message) const = 0;

    // TODO: chat

    /**
     * @brief Makes the player perform the given command
     *
     * @param command Command to perform
     * @return true if the command was successful, otherwise false
     */
    virtual bool performCommand(std::string command) const = 0;  // NOLINT(*-use-nodiscard)

    /**
     * @brief Returns if the player is in sneak mode
     *
     * @return true if player is in sneak mode
     */
    [[nodiscard]] virtual bool isSneaking() const = 0;

    /**
     * @brief Sets the sneak mode of the player
     *
     * @param sneak true if player should appear sneaking
     */
    virtual void setSneaking(bool sneak) = 0;

    /**
     * @brief Gets whether the player is sprinting or not.
     *
     * @return true if player is sprinting.
     */
    [[nodiscard]] virtual bool isSprinting() const = 0;

    /**
     * @brief Sets whether the player is sprinting or not.
     *
     * @param sprinting true if the player should be sprinting
     */
    virtual void setSprinting(bool sprinting) = 0;

    // TODO: playNote

    /**
     * @brief Play a sound for a player at the location.
     *
     * @param location The location to play the sound
     * @param sound The internal sound name to play
     * @param volume The volume of the sound
     * @param pitch The pitch of the sound
     */
    virtual void playSound(Location location, std::string sound, float volume, float pitch) = 0;

    /**
     * @brief Stop the specified sound from playing.
     *
     * @param sound the sound to stop
     */
    virtual void stopSound(std::string sound) = 0;

    /**
     * @brief Stop all sounds from playing.
     */
    virtual void stopAllSounds() = 0;

    // TODO: playEffect

    /**
     * @brief Gives the player the amount of experience specified.
     *
     * @param amount Exp amount to give
     */
    virtual void giveExp(int amount) = 0;

    /**
     * @brief Gives the player the amount of experience levels specified.
     *
     * @param amount amount of experience levels to give or take
     */
    virtual void giveExpLevels(int amount) = 0;

    /**
     * @brief Gets the players current experience progress towards the next level.
     *
     * This is a percentage value. 0.0 is "no progress" and 1.0 is "next level".
     *
     * @return Current experience points
     */
    [[nodiscard]] virtual float getExpProgress() const = 0;

    /**
     * @brief Sets the players current experience progress towards the next level
     *
     * This is a percentage value. 0.0 is "no progress" and 1.0 is "next level".
     *
     * @param progress New experience progress
     */
    virtual void setExpProgress(float progress) = 0;

    /**
     * @brief Gets the players current experience level
     *
     * @return Current experience level
     */
    [[nodiscard]] virtual int getExpLevel() const = 0;

    /**
     * @brief Sets the players current experience level
     *
     * @param level New experience level
     */
    virtual void setExpLevel(int level) = 0;

    /**
     * @brief Gets the players total experience points.
     *
     * This refers to the total amount of experience the player has collected
     * over time and is not currently displayed to the client.
     *
     * @return Current total experience points
     */
    [[nodiscard]] virtual int getTotalExp() const = 0;

    /**
     * @brief Determines if the Player is allowed to fly via jump key double-tap.
     *
     * @return True if the player is allowed to fly.
     */
    [[nodiscard]] virtual bool getAllowFlight() const = 0;

    /**
     * @brief Sets if the Player is allowed to fly via jump key double-tap.
     *
     * @param flight If flight should be allowed.
     */
    virtual void setAllowFlight(bool flight) = 0;

    /**
     * @brief Checks to see if this player is currently flying or not.
     *
     * @return True if the player is flying, else false.
     */
    [[nodiscard]] virtual bool isFlying() const = 0;

    /**
     * @brief Makes this player start or stop flying.
     *
     * @param value True to fly.
     */
    virtual void setFlying(bool value) = 0;

    /**
     * @brief Gets the current allowed speed that a client can fly.
     *
     * @return The current allowed speed, default is 0.05.
     */
    [[nodiscard]] virtual float getFlySpeed() const = 0;

    /**
     * Sets the speed at which a client will fly.
     *
     * @param value The new speed.
     */
    virtual void setFlySpeed(float value) const = 0;

    /**
     * @brief Gets the current allowed speed that a client can walk.
     *
     * @return The current allowed speed, default is 0.10.
     */
    [[nodiscard]] virtual float getWalkSpeed() const = 0;

    /**
     * Sets the speed at which a client will walk.
     *
     * @param value The new speed.
     */
    virtual void setWalkSpeed(float value) const = 0;

    /**
     *@brief Gets the Scoreboard displayed to this player
     *
     * @return The current scoreboard seen by this player
     */
    [[nodiscard]] virtual Scoreboard &getScoreboard() const = 0;

    /**
     * @breif Sets the player's visible Scoreboard.
     *
     * @param scoreboard New Scoreboard for the player
     */
    void virtual setScoreboard(Scoreboard &scoreboard) = 0;

    /**
     * @brief Sends this player a popup message
     *
     * @param message Message to be displayed
     */
    virtual void sendPopup(std::string message) const = 0;

    /**
     * @brief Sends this player a tip message
     *
     * @param message Message to be displayed
     */
    virtual void sendTip(std::string message) const = 0;

    /**
     * @brief Sends this player a toast notification.
     *
     * @param title The title of the toast notification.
     * @param content The content of the toast notification.
     */
    virtual void sendToast(std::string title, std::string content) const = 0;

    /**
     * @brief Sends a title and a subtitle message to the player. If they are empty strings, the display will be
     * updated as such. The titles will be displayed with the default timings.
     *
     * @param title Title text
     * @param subtitle Subtitle text
     */
    virtual void sendTitle(std::string title, std::string subtitle) const = 0;

    /**
     * @brief Sends a title and a subtitle message to the player. If they are empty strings, the display will be
     * updated as such.
     *
     * @param title Title text
     * @param subtitle Subtitle text
     * @param fade_in time in ticks for titles to fade in. Defaults to 10.
     * @param stay time in ticks for titles to stay. Defaults to 70.
     * @param fade_out time in ticks for titles to fade out. Defaults to 20.
     */
    virtual void sendTitle(std::string title, std::string subtitle, int fade_in, int stay, int fade_out) const = 0;

    /**
     * @brief Resets the title displayed to the player. This will clear the displayed title / subtitle and reset
     * timings to their default values.
     */
    virtual void resetTitle() const = 0;

    /**
     * @brief Spawns the particle at the target location.
     *
     * @param name the name of the particle effect to spawn
     * @param location the location to spawn at
     */
    virtual void spawnParticle(std::string name, Location location) const = 0;

    /**
     * @brief Spawns the particle at the target location.
     *
     * @param name the name of the particle effect to spawn
     * @param x the position on the x axis to spawn at
     * @param y the position on the y axis to spawn at
     * @param z the position on the z axis to spawn at
     */
    virtual void spawnParticle(std::string name, float x, float y, float z) const = 0;

    /**
     * @brief Spawns the particle at the target location.
     *
     * @param name the name of the particle effect to spawn
     * @param location the location to spawn at
     * @param molang_variables_json the customizable molang variables that can be adjusted for this particle, in json
     */
    virtual void spawnParticle(std::string name, Location location,
                               std::optional<std::string> molang_variables_json) const = 0;

    /**
     * @brief Spawns the particle at the target location.
     *
     * @param name the name of the particle effect to spawn
     * @param x the position on the x axis to spawn at
     * @param y the position on the y axis to spawn at
     * @param z the position on the z axis to spawn at
     * @param molang_variables_json the customizable molang variables that can be adjusted for this particle, in json
     */
    virtual void spawnParticle(std::string name, float x, float y, float z,
                               std::optional<std::string> molang_variables_json) const = 0;

    // TODO: getClientViewDistance

    /**
     * @brief Gets the player's average ping
     *
     * @return player ping
     */
    [[nodiscard]] virtual std::chrono::milliseconds getPing() const = 0;

    /**
     * @brief Gets the player's current locale.
     *
     * @return the player's locale
     */
    [[nodiscard]] virtual std::string getLocale() const = 0;

    /**
     * @brief Send the list of commands to the client.
     *
     * Generally useful to ensure the client has a complete list of commands after permission changes are done.
     */
    virtual void updateCommands() const = 0;

    // === EntityHuman === //
    /**
     * @brief Get the player's inventory.
     *
     * @return The inventory of the player, this also contains the armor slots.
     */
    [[nodiscard]] virtual PlayerInventory &getInventory() const = 0;

    /**
     * @brief Get the player's EnderChest inventory
     *
     * @return The EnderChest of the player
     */
    [[nodiscard]] virtual Inventory &getEnderChest() const = 0;

    /**
     * @brief Gets this player's current GameMode
     *
     * @return Current game mode
     */
    [[nodiscard]] virtual GameMode getGameMode() const = 0;

    /**
     * @brief Sets this player's current GameMode
     *
     * @param mode New game mode
     */
    virtual void setGameMode(GameMode mode) = 0;

    /**
     * @brief Gets the player's current device's operation system (OS).
     *
     * @return the player's device OS
     */
    [[nodiscard]] virtual std::string getDeviceOS() const = 0;

    /**
     * @brief Gets the player's current device id.
     *
     * @return the player's device id
     */
    [[nodiscard]] virtual std::string getDeviceId() const = 0;

    /**
     * @brief Gets the player's current game version.
     *
     * @return the player's game version
     */
    [[nodiscard]] virtual std::string getGameVersion() const = 0;

    /**
     * @brief Gets the player's skin.
     *
     * @return the player's skin
     */
    [[nodiscard]] virtual Skin getSkin() const = 0;

    /**
     * @brief Sends a form to the player.
     *
     * @param form The form to send
     */
    virtual void sendForm(FormVariant form) = 0;

    /**
     * @brief Closes the forms that are currently open for the player.
     */
    virtual void closeForm() = 0;

    /**
     * @brief Sends a packet to the player.
     *
     * @param packet_id The packet ID to be sent.
     * @param payload The payload of the packet to be transmitted.
     */
    virtual void sendPacket(int packet_id, std::string_view payload) const = 0;
};

}  // namespace endstone
