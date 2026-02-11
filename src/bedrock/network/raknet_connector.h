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

#include "bedrock/deps/raknet/rak_peer_interface.h"
#include "bedrock/network/disconnection_request_info.h"
#include "bedrock/network/network_identifier.h"
#include "bedrock/network/remote_connector.h"

class RakNetConnector : public RemoteConnector {

public:
    struct ConnectionCallbacks : Connector::ConnectionCallbacks {
        virtual void onAllConnectionsClosed(Connection::DisconnectFailReason, bool) = 0;
        virtual void onAllRemoteConnectionsClosed(Connection::DisconnectFailReason, bool) = 0;
        virtual void onOutgoingConnectionFailed(Connection::DisconnectFailReason) = 0;
        virtual void onWebsocketRequest(const std::string &, const std::string &, std::function<void()>) = 0;
    };

    RakNetConnector(ConnectionCallbacks &, RakPeerHelper::IPSupportInterface &,
                    const Bedrock::NonOwnerPointer<AppPlatform> &, const RakNet::RakPeerConfiguration &);
    ~RakNetConnector() override;
    bool host(const ConnectionDefinition &) override;
    bool connect(const Social::GameConnectionInfo &, const Social::GameConnectionInfo &) override;
    void disconnect() override;
    void tick() override;
    void runEvents() override;
    void closeNetworkConnection(const NetworkIdentifier &) override;
    bool setApplicationHandshakeCompleted(const NetworkIdentifier &) override;
    [[nodiscard]] bool isServer() const override;
    std::string getLocalIp() override;
    [[nodiscard]] uint16_t getPort() const override;
    [[nodiscard]] const Social::GameConnectionInfo &getConnectedGameInfo() const override;
    [[nodiscard]] bool isIPv4Supported() const override;
    [[nodiscard]] bool isIPv6Supported() const override;
    [[nodiscard]] uint16_t getIPv4Port() const override;
    [[nodiscard]] uint16_t getIPv6Port() const override;
    [[nodiscard]] NetworkIdentifier getNetworkIdentifier() const override;
    virtual RakNet::RakPeerInterface *getPeer();
    [[nodiscard]] virtual const RakNet::RakPeerInterface *getPeer() const;
    TransportLayer getNetworkType() const override;
    void setDisableLanSignaling(bool) override;

private:
    void _onDisable() override;
    void _onEnable() override;
};
