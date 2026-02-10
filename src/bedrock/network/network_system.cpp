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

#include "bedrock/network/network_system.h"

Bedrock::NotNullNonOwnerPtr<RemoteConnector> NetworkSystem::getRemoteConnector()
{
    return remote_connector_.get();
}

Bedrock::NotNullNonOwnerPtr<const RemoteConnector> NetworkSystem::getRemoteConnector() const
{
    return remote_connector_.get();
}

void NetworkSystem::closeConnection(const NetworkIdentifier &id, const Connection::DisconnectFailReason reason,
                                    const std::string &message_from_server)
{
    onConnectionClosed(id, reason, message_from_server, false, {});
    getRemoteConnector()->closeNetworkConnection(id);
}

const cereal::ReflectionCtx &NetworkSystem::getPacketReflectionCtx() const
{
    return *reflection_ctx_.get();
}

NetworkConnection *NetworkSystem::_getConnectionFromId(const NetworkIdentifier &id) const
{
    for (const auto &connection : connections_) {
        if (connection->id == id) {
            return connection.get();
        }
    }
    return nullptr;
}

void NetworkSystem::_sendInternal(const NetworkIdentifier &id, const Packet &packet, const std::string &data)
{
    const auto *connection = _getConnectionFromId(id);
    if (!connection || connection->shouldCloseConnection()) {
        return;
    }
    if (!connection->peer) {
        return;
    }
    if (packet_observer_) {
        packet_observer_->packetSentTo(id, packet, data.size());
    }
    connection->peer->sendPacket(data, packet.getReliability(), packet.getCompressible());
}
