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

#include <cstddef>
#include <cstdint>
#include <string_view>
#include <vector>

#include "bedrock/deps/webrtc/candidate.h"
#include "bedrock/deps/webrtc/socket.h"

namespace webrtc {

class Connection;
class Network;
class SocketAddress;
class StunMessage;
struct AsyncSocketPacketOptions;
enum IceRole : unsigned int;

enum ProtocolType : unsigned int {
    PROTO_UDP = 0,
    PROTO_TCP = 1,
    PROTO_SSLTCP = 2,
    PROTO_TLS = 3,
    PROTO_LAST = 3,
};

class PortInterface {
public:
    virtual ~PortInterface();
    [[nodiscard]] virtual IceCandidateType Type() const = 0;
    [[nodiscard]] virtual const webrtc::Network *Network() const = 0;
    virtual void SetIceRole(IceRole) = 0;
    [[nodiscard]] virtual IceRole GetIceRole() const = 0;
    virtual void SetIceTiebreaker(std::uint64_t) = 0;
    [[nodiscard]] virtual std::uint64_t IceTiebreaker() const = 0;
    [[nodiscard]] virtual bool SharedSocket() const = 0;
    [[nodiscard]] virtual bool SupportsProtocol(std::string_view) const = 0;
    virtual void PrepareAddress() = 0;
    virtual Connection *GetConnection(const SocketAddress &) = 0;

private:
    enum CandidateOrigin : unsigned int {
        ORIGIN_THIS_PORT = 0,
        ORIGIN_OTHER_PORT = 1,
        ORIGIN_MESSAGE = 2,
    };

public:
    virtual Connection *CreateConnection(const Candidate &, CandidateOrigin) = 0;
    virtual int SetOption(Socket::Option, int) = 0;
    virtual int GetOption(Socket::Option, int *) = 0;
    virtual int GetError() = 0;
    [[nodiscard]] virtual ProtocolType GetProtocol() const = 0;
    [[nodiscard]] virtual const std::vector<Candidate> &Candidates() const = 0;
    virtual int SendTo(const void *, std::size_t, const SocketAddress &, const AsyncSocketPacketOptions &, bool) = 0;
    virtual void SendBindingErrorResponse(StunMessage *, const SocketAddress &, int, std::string_view) = 0;
    // ...
};

}  // namespace webrtc
