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
#include <string>

#include "bedrock/bedrock.h"
#include "bedrock/network/network_peer.h"

namespace endstone::core {
class EndstoneCompressedNetworkPeer;  // Endstone
}  // namespace endstone::core

struct CompressionHeader {
    static constexpr std::size_t SIZE = 1;
    static constexpr std::uint8_t NOT_COMPRESSED = 0xff;

    [[nodiscard]] std::uint8_t getRaw() const
    {
        return raw;
    }
    [[nodiscard]] bool isCompressed() const
    {
        return raw != NOT_COMPRESSED;
    }
    [[nodiscard]] bool isCompressedWith(PacketCompressionAlgorithm algorithm) const
    {
        return raw == static_cast<std::uint8_t>(algorithm);
    }
    void setCompressed(PacketCompressionAlgorithm algorithm)
    {
        raw = static_cast<std::uint8_t>(algorithm);
    }

    std::uint8_t raw = NOT_COMPRESSED;
};

class CompressedNetworkPeer : public NetworkPeer {
protected:
    std::string send_buffer_;
    std::string receive_buffer_;
    bool compression_enabled_;
    bool hybrid_compression_enabled_;
    std::size_t max_decompressed_size_;
    NetworkSettingOptions network_settings_;

    friend class endstone::core::EndstoneCompressedNetworkPeer;  // Endstone
};
BEDROCK_STATIC_ASSERT_SIZE(CompressedNetworkPeer, 128, 112);
