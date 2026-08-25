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

#include "endstone/core/network/compressed_network_peer.h"

#include <algorithm>
#include <utility>

#include <libdeflate.h>
#include <snappy.h>

namespace endstone::core {

namespace {
constexpr int kStoreLevel = 0;
constexpr int kCompressLevel = 6;
constexpr std::size_t kMinInflateGuess = 1024;
}  // namespace

void EndstoneCompressedNetworkPeer::CompressorDeleter::operator()(libdeflate_compressor *p) const
{
    libdeflate_free_compressor(p);
}

void EndstoneCompressedNetworkPeer::DecompressorDeleter::operator()(libdeflate_decompressor *p) const
{
    libdeflate_free_decompressor(p);
}

EndstoneCompressedNetworkPeer::EndstoneCompressedNetworkPeer(std::shared_ptr<NetworkPeer> peer,
                                                             const ::CompressedNetworkPeer &other)
{
    peer_ = std::move(peer);
    compression_enabled_ = other.compression_enabled_;
    hybrid_compression_enabled_ = other.hybrid_compression_enabled_;
    max_decompressed_size_ = other.max_decompressed_size_;
    network_settings_ = other.network_settings_;
}

EndstoneCompressedNetworkPeer::~EndstoneCompressedNetworkPeer() = default;

NetworkPeer::NetworkStatus EndstoneCompressedNetworkPeer::getNetworkStatus() const
{
    return peer_->getNetworkStatus();
}

bool EndstoneCompressedNetworkPeer::deflate(std::string_view input, int level, std::string &out, std::size_t offset)
{
    auto &slot = level == kStoreLevel ? store_compressor_ : compressor_;
    if (!slot) {
        slot.reset(libdeflate_alloc_compressor(level));
        if (!slot) {
            return false;
        }
    }

    out.resize(offset + libdeflate_deflate_compress_bound(slot.get(), input.size()));
    const auto written =
        libdeflate_deflate_compress(slot.get(), input.data(), input.size(), out.data() + offset, out.size() - offset);
    if (written == 0) {
        return false;
    }
    out.resize(offset + written);
    return true;
}

bool EndstoneCompressedNetworkPeer::inflate(std::string_view input, std::string &out)
{
    if (!decompressor_) {
        decompressor_.reset(libdeflate_alloc_decompressor());
        if (!decompressor_) {
            return false;
        }
    }

    auto capacity = std::clamp(input.size() * 6, kMinInflateGuess, std::max(max_decompressed_size_, kMinInflateGuess));
    while (true) {
        out.resize(capacity);
        std::size_t written = 0;
        const auto result = libdeflate_deflate_decompress(decompressor_.get(), input.data(), input.size(), out.data(),
                                                          capacity, &written);
        if (result == LIBDEFLATE_SUCCESS) {
            out.resize(written);
            return true;
        }
        if (result != LIBDEFLATE_INSUFFICIENT_SPACE || capacity >= max_decompressed_size_) {
            return false;
        }
        capacity = std::min(capacity * 2, max_decompressed_size_);
    }
}

void EndstoneCompressedNetworkPeer::sendPacket(const std::string &data, Reliability reliability,
                                               Compressibility compressible)
{
    if (!compression_enabled_) {
        peer_->sendPacket(data, reliability, compressible);
        return;
    }

    const auto threshold = network_settings_.compression_threshold;
    const auto compress = compressible != Compressibility::Incompressible && threshold != 0 && data.size() >= threshold;
    const auto prefix = hybrid_compression_enabled_ ? CompressionHeader::SIZE : 0;

    CompressionHeader header;
    if (!compress) {
        if (hybrid_compression_enabled_) {
            send_buffer_.resize(prefix);
            send_buffer_.append(data);
        }
        else if (!deflate(data, kStoreLevel, send_buffer_, prefix)) {
            return;
        }
    }
    else if (network_settings_.compression_algorithm == PacketCompressionAlgorithm::Snappy) {
        send_buffer_.resize(prefix + snappy::MaxCompressedLength(data.size()));
        std::size_t written = 0;
        snappy::RawCompress(data.data(), data.size(), send_buffer_.data() + prefix, &written);
        send_buffer_.resize(prefix + written);
        header.setCompressed(PacketCompressionAlgorithm::Snappy);
    }
    else {
        if (!deflate(data, kCompressLevel, send_buffer_, prefix)) {
            return;
        }
        header.setCompressed(PacketCompressionAlgorithm::ZLib);
    }

    if (prefix != 0) {
        send_buffer_.front() = static_cast<char>(header.getRaw());
    }
    peer_->sendPacket(send_buffer_, reliability, compressible);
}

NetworkPeer::DataStatus EndstoneCompressedNetworkPeer::_receivePacket(std::string &out_data,
                                                                      const PacketRecvTimepointPtr &timepoint_ptr)
{
    const auto status = peer_->_receivePacket(receive_buffer_, timepoint_ptr);
    if (status != DataStatus::HasData) {
        return status;
    }

    if (!compression_enabled_) {
        out_data = receive_buffer_;
        return DataStatus::HasData;
    }

    if (receive_buffer_.empty()) {
        return DataStatus::BrokenData;
    }

    const auto negotiated = network_settings_.compression_algorithm;
    std::string_view payload{receive_buffer_};
    auto algorithm = negotiated;
    if (hybrid_compression_enabled_) {
        const CompressionHeader header{static_cast<std::uint8_t>(payload.front())};
        payload.remove_prefix(CompressionHeader::SIZE);
        if (!header.isCompressed()) {
            out_data.assign(payload);
            return DataStatus::HasData;
        }
        algorithm = static_cast<PacketCompressionAlgorithm>(header.getRaw());
    }
    else if (static_cast<std::uint16_t>(negotiated) > 0xff) {
        out_data = receive_buffer_;
        return DataStatus::HasData;
    }

    if (algorithm != negotiated) {
        return DataStatus::BrokenData;
    }

    if (algorithm == PacketCompressionAlgorithm::Snappy) {
        std::size_t size = 0;
        if (!snappy::GetUncompressedLength(payload.data(), payload.size(), &size) || size > max_decompressed_size_) {
            return DataStatus::BrokenData;
        }
        out_data.resize(size);
        if (!snappy::RawUncompress(payload.data(), payload.size(), out_data.data())) {
            return DataStatus::BrokenData;
        }
        return DataStatus::HasData;
    }

    if (algorithm != PacketCompressionAlgorithm::ZLib) {
        return DataStatus::BrokenData;
    }
    if (!inflate(payload, out_data)) {
        return DataStatus::BrokenData;
    }
    return DataStatus::HasData;
}

}  // namespace endstone::core
