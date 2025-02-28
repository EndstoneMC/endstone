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

namespace endstone {

/**
 * @brief Called when the server sends a packet to a connected client.
 */
class DataPacketSendEvent : public Cancellable<ServerEvent> {
public:
    DataPacketSendEvent(Player &player, std::string_view data) : player_(player), data_(data) {}

    inline static const std::string NAME = "DataPacketSendEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    /**
     * @brief Gets the raw packet data
     *
     * @return The packet data.
     */
    std::string_view getData() const
    {
        return data_;
    }

    /**
     * Returns the player involved in this event
     *
     * @return Player who is involved in this event
     */
    [[nodiscard]] Player &getPlayer() const
    {
        return player_;
    }

private:
    Player &player_;
    std::string_view data_;
};

}  // namespace endstone
