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
#include <functional>

enum class DevConnectionQuality : int {
    OFF = 0,
    NO_LIMIT = 1,
    PHONE_4G = 2,
    PHONE_3G = 3,
    SLOW = 4,
    VERY_SLOW = 5,
};

enum class Compressibility {
    Compressible = 0,
    Incompressible = 1,
};

enum class PacketCompressionAlgorithm : std::uint16_t {
    ZLib = 0,
    Snappy = 1,
    None = static_cast<std::uint16_t>(-1),
};

struct NetworkSettingOptions {
    NetworkSettingOptions();
    NetworkSettingOptions(uint16_t, PacketCompressionAlgorithm, int, float, bool, bool);
    uint16_t compression_threshold;
    PacketCompressionAlgorithm compression_algorithm;
    bool client_throttle_enabled;
    int client_throttle_threshold;
    float client_throttle_scalar;
    bool raknet_join_flood_protection_enabled;
    bool encryption_disabled;
    DevConnectionQuality dev_connection_quality;
};
static_assert(sizeof(NetworkSettingOptions) == 24);

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
    [[nodiscard]] virtual NetworkStatus getNetworkStatus() const = 0;
    virtual void update() = 0;
    virtual void flush(std::function<void()> &&) = 0;
    [[nodiscard]] virtual bool isLocal() const = 0;
    [[nodiscard]] virtual bool isEncrypted() const = 0;

protected:
    virtual DataStatus _receivePacket(std::string &, const PacketRecvTimepointPtr &) = 0;
};
