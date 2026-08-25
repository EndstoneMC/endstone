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
#include <string>
#include <string_view>

#include "bedrock/network/compressed_network_peer.h"

struct libdeflate_compressor;
struct libdeflate_decompressor;

namespace endstone::core {

class EndstoneCompressedNetworkPeer : public ::CompressedNetworkPeer {
public:
    EndstoneCompressedNetworkPeer(std::shared_ptr<NetworkPeer> peer, const ::CompressedNetworkPeer &other);
    ~EndstoneCompressedNetworkPeer() override;

    void sendPacket(const std::string &data, Reliability reliability, Compressibility compressible) override;
    [[nodiscard]] NetworkStatus getNetworkStatus() const override;

protected:
    DataStatus _receivePacket(std::string &out_data, const PacketRecvTimepointPtr &timepoint_ptr) override;

private:
    struct CompressorDeleter {
        void operator()(libdeflate_compressor *) const;
    };
    struct DecompressorDeleter {
        void operator()(libdeflate_decompressor *) const;
    };

    [[nodiscard]] bool deflate(std::string_view input, int level, std::string &out, std::size_t offset);
    [[nodiscard]] bool inflate(std::string_view input, std::string &out);

    std::unique_ptr<libdeflate_compressor, CompressorDeleter> compressor_;
    std::unique_ptr<libdeflate_compressor, CompressorDeleter> store_compressor_;
    std::unique_ptr<libdeflate_decompressor, DecompressorDeleter> decompressor_;
};

}  // namespace endstone::core
