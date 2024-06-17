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

#include "endstone/actor/actor.h"
#include "endstone/game_mode.h"
#include "endstone/inventory/player_inventory.h"
#include "endstone/util/socket_address.h"
#include "endstone/util/uuid.h"

namespace endstone {

/**
 * @brief Represents a player.
 */
class Player : public Actor {
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
     * @brief Gets the players current percentage towards the next level.
     *
     * This is a percentage value. 0.0 is "no progress" and 1.0 is "next level".
     *
     * @return Current experience points
     */
    [[nodiscard]] virtual float getExp() const = 0;

    /**
     * @brief Gets the players current experience level
     *
     * @return Current experience level
     */
    [[nodiscard]]  virtual int getExpLevel() const = 0;

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
};

}  // namespace endstone
