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

#include "bedrock/network/network_identifier.h"
#include "bedrock/network/network_peer.h"
#include "bedrock/network/disconnection_request_info.h"

class RakNetConnector {

public:
    class ConnectionCallbacks {
    public:
        virtual ~ConnectionCallbacks() = default;
        virtual bool onNewIncomingConnection(const NetworkIdentifier &, std::shared_ptr<NetworkPeer> &&) = 0;
        virtual bool onNewOutgoingConnection(const NetworkIdentifier &, std::shared_ptr<NetworkPeer> &&) = 0;
        virtual void onConnectionClosed(const NetworkIdentifier &, Connection::DisconnectFailReason,
                                        const std::string &, bool) = 0;
        virtual void onAllConnectionsClosed(Connection::DisconnectFailReason, const std::string &, bool) = 0;
        virtual void onAllRemoteConnectionsClosed(Connection::DisconnectFailReason, const std::string &, bool) = 0;
        virtual void onOutgoingConnectionFailed(Connection::DisconnectFailReason, const std::string &) = 0;
        virtual void onWebsocketRequest(const std::string &, const std::string &, std::function<void()>) = 0;
    };
};
