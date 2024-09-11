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

#include "bedrock/bedrock.h"
#include "bedrock/core/memory.h"
#include "bedrock/core/threading.h"
#include "bedrock/core/utility/observer.h"
#include "bedrock/network/connection.h"
#include "bedrock/network/net_event_callback.h"
#include "bedrock/network/network_identifier.h"
#include "bedrock/network/packet/types/connection_request.h"
#include "bedrock/network/packet/types/sub_client_connection_request.h"
#include "bedrock/server/server_player.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/events/server_event.h"
#include "bedrock/world/game_callbacks.h"
#include "bedrock/world/level/level_interface.h"
#include "bedrock/world/level/level_listener.h"
#include "endstone/endstone.h"

namespace endstone::detail {
class EndstoneServer;
}

enum class ServerTextEvent : std::uint8_t {
    Sleeping = 0,
    Connection = 1,
    ChangedSkin = 2,
    ConnectionOriginal = Connection | 0x80,  // Endstone
};

class ServerNetworkHandler : public Bedrock::Threading::EnableQueueForMainThread,
                             public NetEventCallback,
                             public LevelListener,
                             public Social::MultiplayerServiceObserver,
                             public Social::XboxLiveUserObserver {
public:
    ~ServerNetworkHandler() override = 0;
    
    ENDSTONE_HOOK bool trytLoadPlayer(ServerPlayer &, ConnectionRequest const &);
    ENDSTONE_HOOK void disconnectClient(NetworkIdentifier const &, SubClientId, Connection::DisconnectFailReason,
                                        std::string const &, std::optional<std::string>, bool);
    ENDSTONE_HOOK void updateServerAnnouncement();

    [[nodiscard]] const Bedrock::NonOwnerPointer<ILevel> &getLevel() const;  // Endstone

private:
    friend class endstone::detail::EndstoneServer;
    // NOLINTBEGIN(*-identifier-naming)
    ENDSTONE_HOOK ServerPlayer &_createNewPlayer(NetworkIdentifier const &, SubClientConnectionRequest const &,
                                                 SubClientId);
    ENDSTONE_HOOK void _displayGameMessage(Player const &, ChatEvent &);
    [[nodiscard]] ENDSTONE_HOOK bool _isServerTextEnabled(ServerTextEvent const &) const;
    // NOLINTEND(*-identifier-naming)

    GameCallbacks *callbacks_;                               // +80
    Bedrock::NonOwnerPointer<ILevel> level_;                 // +88
    std::size_t pad_[ENDSTONE_VARIANT_WIN32_LINUX(92, 75)];  // +104
    int max_players_;                                        // +840 (+704)
};
