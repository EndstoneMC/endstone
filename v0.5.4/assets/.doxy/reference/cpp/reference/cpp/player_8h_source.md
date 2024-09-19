

# File player.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**player.h**](player_8h.md)

[Go to the documentation of this file](player_8h.md)


```C++
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
#include "endstone/network/spawn_particle_effect_packet.h"
#include "endstone/scoreboard/scoreboard.h"
#include "endstone/skin.h"
#include "endstone/util/socket_address.h"
#include "endstone/util/uuid.h"

namespace endstone {

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

    [[nodiscard]] virtual endstone::UUID getUniqueId() const = 0;

    [[nodiscard]] virtual std::string getXuid() const = 0;

    [[nodiscard]] virtual const SocketAddress &getAddress() const = 0;

    virtual void sendPopup(std::string message) const = 0;

    virtual void sendTip(std::string message) const = 0;

    virtual void sendToast(std::string title, std::string content) const = 0;

    virtual void kick(std::string message) const = 0;

    virtual void giveExp(int amount) = 0;

    virtual void giveExpLevels(int amount) = 0;

    [[nodiscard]] virtual float getExpProgress() const = 0;

    virtual void setExpProgress(float progress) = 0;

    [[nodiscard]] virtual int getExpLevel() const = 0;

    virtual void setExpLevel(int level) = 0;

    [[nodiscard]] virtual int getTotalExp() const = 0;

    [[nodiscard]] virtual bool getAllowFlight() const = 0;

    virtual void setAllowFlight(bool flight) = 0;

    [[nodiscard]] virtual bool isFlying() const = 0;

    virtual void setFlying(bool value) = 0;

    [[nodiscard]] virtual float getFlySpeed() const = 0;

    virtual void setFlySpeed(float value) const = 0;

    [[nodiscard]] virtual float getWalkSpeed() const = 0;

    virtual void setWalkSpeed(float value) const = 0;

    [[nodiscard]] virtual Scoreboard &getScoreboard() const = 0;

    void virtual setScoreboard(Scoreboard &scoreboard) = 0;

    virtual void sendTitle(std::string title, std::string subtitle) const = 0;

    virtual void sendTitle(std::string title, std::string subtitle, int fade_in, int stay, int fade_out) const = 0;

    virtual void resetTitle() const = 0;

    [[nodiscard]] virtual std::chrono::milliseconds getPing() const = 0;

    virtual void updateCommands() const = 0;

    virtual bool performCommand(std::string command) const = 0;  // NOLINT(*-use-nodiscard)

    [[nodiscard]] virtual GameMode getGameMode() const = 0;

    virtual void setGameMode(GameMode mode) = 0;

    [[nodiscard]] virtual PlayerInventory &getInventory() const = 0;

    [[nodiscard]] virtual std::string getLocale() const = 0;

    [[nodiscard]] virtual std::string getDeviceOS() const = 0;

    [[nodiscard]] virtual std::string getDeviceId() const = 0;

    [[nodiscard]] virtual std::string getGameVersion() const = 0;

    [[nodiscard]] virtual const Skin &getSkin() const = 0;

    virtual void transfer(std::string host, int port) const = 0;

    virtual void sendForm(FormVariant form) = 0;

    virtual void closeForm() = 0;

    virtual void sendPacket(Packet &packet) = 0;
};

}  // namespace endstone
```


