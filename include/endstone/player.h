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
#include <variant>

#include "endstone/actor/mob.h"
#include "endstone/form/action_form.h"
#include "endstone/form/message_form.h"
#include "endstone/form/modal_form.h"
#include "endstone/game_mode.h"
#include "endstone/inventory/player_inventory.h"
#include "endstone/skin.h"
#include "endstone/util/socket_address.h"
#include "endstone/util/uuid.h"

namespace endstone {

/**
 * @brief Represents a player.
 */
class Player : public Mob {
protected:
    using FormVariant = std::variant<MessageForm, ActionForm, ModalForm>;

public:
    Player() = default;
    Player(const Player &) = delete;
    Player &operator=(const Player &) = delete;
    Player(Player &&) = delete;
    Player &operator=(Player &&) = delete;

    ~Player() override = default;

    // CommandSender
    [[nodiscard]] Player *asPlayer() const override
    {
        return const_cast<Player *>(this);
    }

    /**
     * @brief Returns the UUID of this player
     *
     * @return Player UUID
     */
    [[nodiscard]] virtual endstone::UUID getUniqueId() const = 0;

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
    [[nodiscard]] virtual const SocketAddress &getAddress() const = 0;

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
     * @brief Kicks player with custom kick message.
     *
     * @param message kick message
     */
    virtual void kick(std::string message) const = 0;

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
     * @brief Gets the player's average ping
     *
     * @return player ping
     */
    [[nodiscard]] virtual std::chrono::milliseconds getPing() const = 0;

    /**
     * @brief Send the list of commands to the client.
     *
     * Generally useful to ensure the client has a complete list of commands after permission changes are done.
     */
    virtual void updateCommands() const = 0;

    /**
     * @brief Makes the player perform the given command
     *
     * @param command Command to perform
     * @return true if the command was successful, otherwise false
     */
    virtual bool performCommand(std::string command) const = 0;  // NOLINT(*-use-nodiscard)

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
     * @brief Get the player's inventory.
     *
     * @return The inventory of the player, this also contains the armor slots.
     */
    [[nodiscard]] virtual PlayerInventory &getInventory() const = 0;

    /**
     * @brief Gets the player's current locale.
     *
     * @return the player's locale
     */
    [[nodiscard]] virtual std::string getLocale() const = 0;

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
    [[nodiscard]] virtual endstone::UUID getDeviceId() const = 0;

    /**
     * @brief Gets the player's skin.
     *
     * @return the player's skin
     */
    [[nodiscard]] virtual const Skin &getSkin() const = 0;

    /**
     * @brief Transfers the player to another server
     *
     * @param address Server address to transfer the player to.
     * @param port Server port to transfer the player to
     */
    virtual void transfer(std::string address, int port) const = 0;

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
};

}  // namespace endstone
