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

#include "bedrock/deps/raknet/packet_priority.h"
#include "bedrock/deps/raknet/raknet_types.h"

// NOLINTBEGIN
namespace RakNet {
class BitStream;
class InternalPacket;
class TCPInterface;
class RakPeerInterface;

enum PluginReceiveResult : unsigned int {
    RR_STOP_PROCESSING_AND_DEALLOCATE = 0,
    RR_CONTINUE_PROCESSING = 1,
    RR_STOP_PROCESSING = 2,
};

enum PI2_LostConnectionReason : unsigned int {
    LCR_CLOSED_BY_USER = 0,
    LCR_DISCONNECTION_NOTIFICATION = 1,
    LCR_CONNECTION_LOST = 2,
};

enum PI2_FailedConnectionAttemptReason : unsigned int {
    FCAR_CONNECTION_ATTEMPT_FAILED = 0,
    FCAR_ALREADY_CONNECTED = 1,
    FCAR_NO_FREE_INCOMING_CONNECTIONS = 2,
    FCAR_SECURITY_PUBLIC_KEY_MISMATCH = 3,
    FCAR_CONNECTION_BANNED = 4,
    FCAR_INVALID_PASSWORD = 5,
    FCAR_INCOMPATIBLE_PROTOCOL = 6,
    FCAR_IP_RECENTLY_CONNECTED = 7,
    FCAR_REMOTE_SYSTEM_REQUIRES_PUBLIC_KEY = 8,
    FCAR_OUR_SYSTEM_REQUIRES_SECURITY = 9,
    FCAR_PUBLIC_KEY_MISMATCH = 10,
};

class PluginInterface2 {
public:
    PluginInterface2() = default;
    virtual ~PluginInterface2() = default;
    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void Update() {}
    virtual PluginReceiveResult OnReceive(Packet *packet) { return RR_CONTINUE_PROCESSING; }
    virtual void OnRakPeerStartup() {}
    virtual void OnRakPeerShutdown() {}
    virtual void OnClosedConnection(const SystemAddress &systemAddress, RakNetGUID rakNetGUID,
                                    PI2_LostConnectionReason lostConnectionReason)
    {
    }
    virtual void OnNewConnection(const SystemAddress &systemAddress, RakNetGUID rakNetGUID, bool isIncoming) {}
    virtual void OnFailedConnectionAttempt(Packet *packet,
                                           PI2_FailedConnectionAttemptReason failedConnectionAttemptReason)
    {
    }
    virtual bool UsesReliabilityLayer() const { return false; }
    virtual void OnDirectSocketSend(const char *data, const BitSize_t bitsUsed, SystemAddress remoteSystemAddress) {}
    virtual void OnDirectSocketReceive(const char *data, const BitSize_t bitsUsed, SystemAddress remoteSystemAddress) {}
    virtual void OnReliabilityLayerNotification(const char *errorMessage, const BitSize_t bitsUsed,
                                                SystemAddress remoteSystemAddress, bool isError)
    {
    }
    virtual void OnInternalPacket(InternalPacket *internalPacket, unsigned int frameNumber,
                                  SystemAddress remoteSystemAddress, TimeMS time, int isSend)
    {
    }
    virtual void OnAck(unsigned int messageNumber, SystemAddress remoteSystemAddress, TimeMS time) {}
    virtual void OnPushBackPacket(const char *data, const BitSize_t bitsUsed, SystemAddress remoteSystemAddress) {}
    RakPeerInterface *GetRakPeerInterface() const;
    RakNetGUID GetMyGUIDUnified() const;
    void SetRakPeerInterface(RakPeerInterface *ptr);
    void SetTCPInterface(TCPInterface *ptr);

protected:
    void SendUnified(const BitStream *bitStream, PacketPriority priority, PacketReliability reliability,
                     char orderingChannel, const AddressOrGUID systemIdentifier, bool broadcast);
    void SendUnified(const char *data, const int length, PacketPriority priority, PacketReliability reliability,
                     char orderingChannel, const AddressOrGUID systemIdentifier, bool broadcast);
    bool SendListUnified(const char **data, const int *lengths, const int numParameters, PacketPriority priority,
                         PacketReliability reliability, char orderingChannel, const AddressOrGUID systemIdentifier,
                         bool broadcast);
    Packet *AllocatePacketUnified(unsigned int dataSize);
    void PushBackPacketUnified(Packet *packet, bool pushAtHead);
    void DeallocPacketUnified(Packet *packet);
    RakPeerInterface *rakPeerInterface;
    TCPInterface *tcpInterface;
};
}  // namespace RakNet
// NOLINTEND
