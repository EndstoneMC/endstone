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

#include "bedrock/network/network_system_toggles.h"

class Connector {
public:
    struct ConnectionCallbacks {
        virtual ~ConnectionCallbacks() = default;
        virtual bool onNewIncomingConnection(NetworkIdentifier const &, std::shared_ptr<::NetworkPeer> &&) = 0;
        virtual bool onNewOutgoingConnection(NetworkIdentifier const &, std::shared_ptr<::NetworkPeer> &&) = 0;
        virtual void onConnectionClosed(NetworkIdentifier const &id, Connection::DisconnectFailReason disconnect_reason,
                                        std::string const &message_from_server, std::string &message_body_override,
                                        bool skip_disconnect_message, const Json::Value &session_summary) = 0;
    };

    Connector(ConnectionCallbacks &);
    virtual ~Connector();
    virtual std::string getLocalIp();
    [[nodiscard]] virtual uint16_t getPort() const;
    [[nodiscard]] virtual const Social::GameConnectionInfo &getConnectedGameInfo() const;
    [[nodiscard]] virtual bool isIPv4Supported() const;
    [[nodiscard]] virtual bool isIPv6Supported() const;
    [[nodiscard]] virtual uint16_t getIPv4Port() const;
    [[nodiscard]] virtual uint16_t getIPv6Port() const;
    [[nodiscard]] virtual TransportLayer getNetworkType() const;

protected:
    ConnectionCallbacks &callbacks_;
};
static_assert(sizeof(Connector) == 16);
