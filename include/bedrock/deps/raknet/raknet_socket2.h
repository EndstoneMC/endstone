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

#include <future>
#include <memory>

#include "bedrock/bedrock.h"
#include "bedrock/deps/raknet/raknet_defines.h"
#include "bedrock/deps/raknet/raknet_types.h"
// must be included after raknet_types.h
#include "bedrock/deps/raknet/lockless_types.h"

/**
 * https://github.com/facebookarchive/RakNet/blob/master/Source/RakNetSocket2.h
 */

namespace RakNet {

using RNS2SendResult = int;
using RNS2Socket = int;
enum class RNS2Type;

struct RNS2_SendParameters {  // NOLINT
    RNS2_SendParameters()
    {
        ttl = 0;
    }
    char *data;
    int length;
    SystemAddress system_address;
    int ttl;
};

class RakNetSocket2;
struct RNS2RecvStruct {
    char data[MAXIMUM_MTU_SIZE];   // +0
    int bytes_read;                // +1600
    SystemAddress system_address;  // +1608
    RakNet::TimeUS time_read;      // +1744
    RakNetSocket2 *socket;         // +1752
};

class RNS2EventHandler {
public:
    virtual ~RNS2EventHandler() = 0;
    virtual void OnRNS2Recv(RNS2RecvStruct *recv_struct) = 0;                                        // NOLINT
    virtual void DeallocRNS2RecvStruct(RNS2RecvStruct *s, const char *file, unsigned int line) = 0;  // NOLINT
    virtual RNS2RecvStruct *AllocRNS2RecvStruct(const char *file, unsigned int line) = 0;            // NOLINT
};

struct RNS2_BerkleyBindParameters {  // NOLINT
    unsigned short port;
    char *host_address;
    std::uint16_t address_family;  // AF_INET or AF_INET6
    int type;                      // SOCK_DGRAM
    int protocol;                  // 0
    bool non_blocking_socket;
    int set_broadcast;
    int set_ip_hdr_incl;
    int do_not_fragment;
    int polling_thread_priority;
    RNS2EventHandler *event_handler;
    std::uint16_t remote_port_raknet_was_started_on_ps3_ps4_psp2;
};

class RNS2_Windows_Linux_360 {  // NOLINT
protected:
    // NOLINTNEXTLINE
    ENDSTONE_HOOK static RNS2SendResult Send_Windows_Linux_360NoVDP(RNS2Socket socket,
                                                                    RNS2_SendParameters *send_parameters,
                                                                    const char *file, unsigned int line);
};

class RakNetSocket2 : std::enable_shared_from_this<RakNetSocket2> {
public:
    virtual ~RakNetSocket2() = 0;

protected:
    RNS2EventHandler *event_handler_;            // +24
    RNS2Type socket_type_;                       // +32
    int socket_protocol_type_;                   // +36
    SystemAddress bound_address_;                // +40
    unsigned int user_connection_socket_index_;  // +176
};

class IRNS2_Berkley : public RakNetSocket2 {  // NOLINT
public:
    virtual ~IRNS2_Berkley() override = 0;
};

class RNS2_Berkley : public IRNS2_Berkley {  // NOLINT
public:
    virtual ~RNS2_Berkley() override = 0;

protected:
    void RecvFromBlocking(RNS2RecvStruct *recvFromStruct) const;
    ENDSTONE_HOOK unsigned int RecvFromLoopInt();

    RNS2Socket rns2_socket_;                                    // +184 (+180)
    RNS2_BerkleyBindParameters binding_;                        // +192 (+184)
    RakNet::LocklessUint32_t is_recv_from_loop_thread_active_;  // +256 (+248)
    volatile bool end_threads_;                                 // +260 (+252)
};

}  // namespace RakNet
