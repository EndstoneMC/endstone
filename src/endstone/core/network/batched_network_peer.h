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

#include <functional>
#include <memory>
#include <string>
#include <string_view>

#include "bedrock/network/batched_network_peer.h"
#include "bedrock/network/network_identifier.h"

namespace endstone::core {

class EndstoneBatchedNetworkPeer : public ::BatchedNetworkPeer {
public:
    EndstoneBatchedNetworkPeer(NetworkIdentifier id, std::shared_ptr<NetworkPeer> peer,
                               std::shared_ptr<::BatchedNetworkPeer> async_peer);

    void sendPacket(const std::string &data, Reliability reliability, Compressibility compressible) override;
    [[nodiscard]] NetworkStatus getNetworkStatus() const override;
    void update() override;
    void flush(std::function<void()> &&callback) override;

protected:
    DataStatus _receivePacket(std::string &out_data, const PacketRecvTimepointPtr &timepoint_ptr) override;

private:
    void writePacket(std::string_view data, Compressibility compressible);
    [[nodiscard]] DataStatus readPacket(std::string &out_data, const PacketRecvTimepointPtr &timepoint_ptr);
    [[nodiscard]] ::BatchedNetworkPeer &batchingPeer();

    NetworkIdentifier id_;
    std::shared_ptr<::BatchedNetworkPeer> async_peer_;
};

}  // namespace endstone::core
