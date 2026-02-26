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

#include <string>
#include <string_view>
#include <utility>

#include "endstone/event/cancellable.h"
#include "endstone/event/server/server_event.h"

namespace endstone {

/**
 * @brief Called when the server receives a packet from a connected client.
 */
class PacketReceiveEvent : public Cancellable<ServerEvent> {
public:
    ENDSTONE_EVENT(PacketReceiveEvent);
    PacketReceiveEvent(Player *player, const int packet_id, std::string_view payload, SocketAddress address,
                       const int sub_client_id)
        : player_(player), packet_id_(packet_id), payload_(payload), address_(std::move(address)),
          sub_client_id_(sub_client_id)
    {
    }

    /**
     * @brief Gets the ID of the packet.
     *
     * @return The packet ID.
     */
    [[nodiscard]] int getPacketId() const { return packet_id_; }

    /**
     * @brief Gets the raw packet data **excluding** the header.
     *
     * @return The packet payload data.
     */
    [[nodiscard]] std::string_view getPayload() const { return payload_; }

    /**
     * @brief Sets the raw packet data **excluding** the header.
     *
     * @return The packet payload data.
     */
    void setPayload(std::string_view payload)
    {
        owned_payload_ = payload;
        payload_ = owned_payload_;
    }

    /**
     * @brief Returns the player involved in this event
     *
     * @note This may return nullptr if the packet is sent before the player completes the login process.
     *
     * @return Player who is involved in this event
     */
    [[nodiscard]] Player *getPlayer() const { return player_; }

    /**
     * @brief Gets the network address to which this packet is being sent.
     *
     * @return The SocketAddress of the destination client.
     */
    [[nodiscard]] SocketAddress getAddress() const { return address_; }

    /**
     * @brief Gets the SubClient ID.
     *
     * @note Range is 0 to 3 (0 = primary client; 1-3 = split-screen clients).
     *
     * @return The SubClient ID.
     */
    [[nodiscard]] int getSubClientId() const { return sub_client_id_; }

private:
    Player *player_;
    int packet_id_;
    std::string_view payload_;
    std::string owned_payload_;
    SocketAddress address_;
    int sub_client_id_;
};

}  // namespace endstone
