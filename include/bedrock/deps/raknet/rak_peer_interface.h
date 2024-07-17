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

#include <memory>

#include "bedrock/deps/raknet/raknet_socket2.h"
#include "bedrock/deps/raknet/raknet_types.h"

namespace RakNet {

class BitStream;
class Packet;
struct PublicKey;
class RakNetSocket2;
class ShadowBanList;
class RakNetStatistics;
class SocketDescriptor;
class PluginInterface2;
enum class ConnectionAttemptResult;
enum class ConnectionState;
enum class PacketPriority;
enum class PacketReliability;

namespace DataStructures {
template <typename T>
class List;
}

// NOLINTBEGIN
class RakPeerInterface {
public:
    virtual ~RakPeerInterface() = 0;
    virtual void InitializeConfiguration(std::unique_ptr<ShadowBanList>) = 0;
    virtual StartupResult Startup(unsigned int, SocketDescriptor *, unsigned, int) = 0;
    virtual bool InitializeSecurity(const char *, const char *, bool) = 0;
    virtual void DisableSecurity() = 0;
    virtual void AddToSecurityExceptionList(char const *) = 0;
    virtual void RemoveFromSecurityExceptionList(char const *) = 0;
    virtual bool IsInSecurityExceptionList(char const *) = 0;
    virtual void SetMaximumIncomingConnections(unsigned short) = 0;
    virtual unsigned int GetMaximumIncomingConnections() const = 0;
    virtual unsigned short NumberOfConnections() const = 0;
    virtual void SetIncomingPassword(char const *, int) = 0;
    virtual void GetIncomingPassword(char *, int *) = 0;
    virtual ConnectionAttemptResult Connect(char const *, unsigned short, char const *, int, PublicKey *, unsigned int,
                                            unsigned int, unsigned int, unsigned int) = 0;
    virtual ConnectionAttemptResult ConnectWithSocket(char const *, unsigned short, char const *, int, RakNetSocket2 *,
                                                      PublicKey *, unsigned int, unsigned int, unsigned int) = 0;
    virtual void Shutdown(unsigned int, unsigned char, PacketPriority) = 0;
    virtual bool SetApplicationHandshakeCompleted(AddressOrGUID) = 0;
    virtual bool IsActive() const = 0;
    virtual bool GetConnectionList(SystemAddress *, unsigned short *) const = 0;
    virtual unsigned int GetNextSendReceipt() = 0;
    virtual unsigned int IncrementNextSendReceipt() = 0;
    virtual unsigned int Send(BitStream const *, PacketPriority, PacketReliability, char, AddressOrGUID, bool,
                              unsigned int) = 0;
    virtual unsigned int Send(char const *, int, PacketPriority, PacketReliability, char, AddressOrGUID, bool,
                              unsigned int) = 0;
    virtual void SendLoopback(char const *, int) = 0;
    virtual unsigned int SendList(char const **, int const *, int, PacketPriority, PacketReliability, char,
                                  AddressOrGUID, bool, unsigned int) = 0;
    virtual Packet *Receive() = 0;
    virtual void DeallocatePacket(Packet *) = 0;
    virtual unsigned int GetMaximumNumberOfPeers() const = 0;
    virtual void CloseConnection(AddressOrGUID, bool, unsigned char, PacketPriority) = 0;
    virtual ConnectionState GetConnectionState(AddressOrGUID) = 0;
    virtual void CancelConnectionAttempt(SystemAddress) = 0;
    virtual int GetIndexFromSystemAddress(SystemAddress) const = 0;
    virtual SystemAddress GetSystemAddressFromIndex(unsigned int) = 0;
    virtual RakNetGUID GetGUIDFromIndex(unsigned int) = 0;
    virtual void GetSystemList(DataStructures::List<SystemAddress> &, DataStructures::List<RakNetGUID> &) const = 0;
    virtual void AddToBanList(char const *, unsigned int) = 0;
    virtual void RemoveFromBanList(char const *) = 0;
    virtual void ClearBanList() = 0;
    virtual bool IsBanned(char const *) = 0;
    virtual void SetLimitIPConnectionFrequency(bool) = 0;
    virtual bool Ping(char const *, unsigned short, bool, unsigned int) = 0;
    virtual void Ping(SystemAddress) = 0;
    virtual void SendNatTraversalMessage(SystemAddress) = 0;
    virtual int GetAveragePing(AddressOrGUID) = 0;
    virtual int GetLastPing(AddressOrGUID) const = 0;
    virtual int GetLowestPing(AddressOrGUID) const = 0;
    virtual void SetOccasionalPing(bool) = 0;
    virtual std::uint64_t GetClockDifferential(AddressOrGUID) = 0;
    virtual void SetOfflinePingResponse(char const *, unsigned int) = 0;
    virtual void GetOfflinePingResponse(char **, unsigned int *) = 0;
    virtual SystemAddress GetInternalID(SystemAddress, int) const = 0;
    virtual void SetInternalID(SystemAddress, int) = 0;
    virtual SystemAddress GetExternalID(SystemAddress) const = 0;
    virtual RakNetGUID const GetMyGUID() const = 0;
    virtual void resetMyGUID() = 0;
    virtual SystemAddress GetMyBoundAddress(int) = 0;
    virtual RakNetGUID const &GetGuidFromSystemAddress(SystemAddress) const = 0;
    virtual SystemAddress GetSystemAddressFromGuid(const RakNetGUID) const = 0;
    virtual bool GetClientPublicKeyFromSystemAddress(const SystemAddress, char *) const = 0;
    virtual void SetTimeoutTime(RakNet::TimeMS, const SystemAddress) = 0;
    virtual RakNet::TimeMS GetTimeoutTime(const SystemAddress) = 0;
    virtual int GetMTUSize(const SystemAddress) const = 0;
    virtual unsigned GetNumberOfAddresses() = 0;
    virtual const char *GetLocalIP(unsigned int) = 0;
    virtual bool IsLocalIP(const char *) = 0;
    virtual void AllowConnectionResponseIPMigration(bool allow) = 0;
    virtual bool AdvertiseSystem(const char *, unsigned short, const char *, int, unsigned = 0) = 0;
    virtual void SetSplitMessageProgressInterval(int) = 0;
    virtual int GetSplitMessageProgressInterval() const = 0;
    virtual void SetUnreliableTimeout(RakNet::TimeMS) = 0;
    virtual void SendTTL(const char *, unsigned short, int, unsigned = 0) = 0;
    virtual void AttachPlugin(PluginInterface2 *) = 0;
    virtual void DetachPlugin(PluginInterface2 *) = 0;
    virtual void PushBackPacket(Packet *, bool) = 0;
    virtual void ChangeSystemAddress(RakNetGUID, const SystemAddress &) = 0;
    virtual Packet *AllocatePacket(unsigned) = 0;
    virtual RakNetSocket2 *GetSocket(const SystemAddress) = 0;
    virtual void GetSockets(DataStructures::List<RakNetSocket2 *> &) = 0;
    virtual void ReleaseSockets(DataStructures::List<RakNetSocket2 *> &) = 0;
    virtual void WriteOutOfBandHeader(RakNet::BitStream *) = 0;
    virtual void SetUserUpdateThread(void (*)(RakPeerInterface *, void *), void *) = 0;
    virtual void SetIncomingDatagramEventHandler(bool (*)(RNS2RecvStruct *)) = 0;
    virtual void ApplyNetworkSimulator(float, unsigned short, unsigned short) = 0;
    virtual void SetPerConnectionOutgoingBandwidthLimit(unsigned) = 0;
    virtual bool IsNetworkSimulatorActive() = 0;
    virtual RakNetStatistics *GetStatistics(const SystemAddress, RakNetStatistics * = 0) = 0;
    virtual bool GetStatistics(const unsigned int, RakNetStatistics *) = 0;
    virtual void GetStatisticsList(DataStructures::List<SystemAddress> &, DataStructures::List<RakNetGUID> &,
                                   DataStructures::List<RakNetStatistics> &) = 0;
    virtual unsigned int GetReceiveBufferSize() = 0;
};
// NOLINTEND

}  // namespace RakNet
