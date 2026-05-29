

# File packet\_receive\_event.h

[**File List**](files.md) **>** [**endstone**](dir_6cf277b678674f97c7a2b6b3b2447b33.md) **>** [**event**](dir_f1d783c0ad83ee143d16e768ebca51c8.md) **>** [**server**](dir_77022909323d5ad872c4820a738a5429.md) **>** [**packet\_receive\_event.h**](packet__receive__event_8h.md)

[Go to the documentation of this file](packet__receive__event_8h.md)


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

#include "endstone/event/cancellable.h"
#include "endstone/event/server/server_event.h"

namespace endstone {

class PacketReceiveEvent : public Cancellable<ServerEvent> {
public:
    PacketReceiveEvent(Player *player, const int packet_id, std::string_view payload, SocketAddress address,
                       const int sub_client_id)
        : player_(player), packet_id_(packet_id), payload_(payload), address_(std::move(address)),
          sub_client_id_(sub_client_id)
    {
    }

    inline static const std::string NAME = "PacketReceiveEvent";
    [[nodiscard]] std::string getEventName() const override
    {
        return NAME;
    }

    [[nodiscard]] int getPacketId() const
    {
        return packet_id_;
    }

    [[nodiscard]] std::string_view getPayload() const
    {
        return payload_;
    }

    void setPayload(std::string_view payload)
    {
        owned_payload_ = payload;
        payload_ = owned_payload_;
    }

    [[nodiscard]] Player *getPlayer() const
    {
        return player_;
    }

    [[nodiscard]] SocketAddress getAddress() const
    {
        return address_;
    }

    [[nodiscard]] int getSubClientId() const
    {
        return sub_client_id_;
    }

private:
    Player *player_;
    int packet_id_;
    std::string_view payload_;
    std::string owned_payload_;
    SocketAddress address_;
    int sub_client_id_;
};

}  // namespace endstone
```


