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
#include <chrono>

enum class Compressibility {
    Compressible = 0,
    Incompressible = 1,
};

class NetworkPeer {
public:
    using PacketRecvTimepoint = std::chrono::steady_clock::time_point;
    using PacketRecvTimepointPtr = std::shared_ptr<PacketRecvTimepoint>;

    enum class Reliability {
        Reliable = 0,
        ReliableOrdered = 1,
        Unreliable = 2,
        UnreliableSequenced = 3,
    };
    enum class DataStatus : int {
        HasData = 0,
        NoData = 1,
        BrokenData = 2,
    };
    struct NetworkStatus {};

    virtual ~NetworkPeer() = default;
    virtual void sendPacket(const std::string &, Reliability, Compressibility) = 0;
    virtual DataStatus receivePacket(std::string &, const PacketRecvTimepointPtr &) = 0;
    virtual NetworkStatus getNetworkStatus() const = 0;
    virtual void update() = 0;
    virtual void flush(std::function<void()> &&) = 0;
    virtual bool isLocal() const = 0;
    virtual bool isEncrypted() const = 0;
};
