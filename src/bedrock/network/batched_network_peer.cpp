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

#include "bedrock/network/batched_network_peer.h"

NetworkPeer::NetworkStatus BatchedNetworkPeer::getNetworkStatus() const
{
    return peer_->getNetworkStatus();
}

bool BatchedNetworkPeer::isLocal() const
{
    return peer_->isLocal();
}

bool BatchedNetworkPeer::isEncrypted() const
{
    return peer_->isEncrypted();
}

bool BatchedNetworkPeer::isLan() const
{
    return peer_->isLan();
}

Compressibility BatchedNetworkPeer::getCompressibleState(std::size_t data_to_send) const
{
    if (data_to_send == 0) {
        return Compressibility::Incompressible;
    }
    const auto ratio = static_cast<float>(compressible_bytes_) / static_cast<float>(data_to_send);
    return ratio <= 0.2F ? Compressibility::Incompressible : Compressibility::Compressible;
}

void BatchedNetworkPeer::sendPacket(const std::string &data, Reliability /*reliability*/, Compressibility compressible)
{
    outgoing_data_.writeString(data, "Data", nullptr);
    if (compressible == Compressibility::Compressible) {
        compressible_bytes_ += data.size();
    }
    if (getCompressibleState(outgoing_data_.getBuffer().size()) == Compressibility::Incompressible) {
        flush({});
    }
}

void BatchedNetworkPeer::flush(std::function<void()> &&callback)
{
    std::string data_to_send = outgoing_data_.getAndReleaseData();
    // BDS hands the batch to a TaskGroup send task when mAsyncEnabled; Endstone supersedes that path in
    // AsyncBatchedNetworkPeer, so here we only do BDS's synchronous flush.
    if (!data_to_send.empty()) {
        peer_->sendPacket(data_to_send, Reliability::ReliableOrdered, getCompressibleState(data_to_send.size()));
        compressible_bytes_ = 0;
    }
    if (callback) {
        callback();
    }
}

void BatchedNetworkPeer::update()
{
    flush({});
    if (peer_) {
        peer_->update();
    }
}

NetworkPeer::DataStatus BatchedNetworkPeer::_receivePacket(std::string &out_data,
                                                           const PacketRecvTimepointPtr &timepoint_ptr)
{
    auto status = DataStatus::NoData;
    if (!incoming_data_ || incoming_data_->getUnreadLength() == 0) {
        const auto received = peer_->_receivePacket(incoming_data_buffer_, timepoint_ptr);
        if (received != DataStatus::HasData) {
            return received;
        }
        if (incoming_data_buffer_.empty()) {
            return DataStatus::BrokenData;
        }
        incoming_data_.emplace(incoming_data_buffer_, false);
    }
    if (incoming_data_->getUnreadLength() != 0) {
        out_data.clear();
        if (const auto length = incoming_data_->getUnsignedVarInt().discardError()) {
            const auto view = incoming_data_->getView();
            const auto start = incoming_data_->getReadPointer();
            if (start + length.value() <= view.size()) {
                out_data.assign(view.substr(start, length.value()));
                incoming_data_->setReadPointer(start + length.value());
            }
        }
        status = DataStatus::HasData;
    }
    if (incoming_data_->hasOverflowed()) {
        incoming_data_buffer_.clear();
        incoming_data_.emplace(incoming_data_buffer_, false);
        return DataStatus::BrokenData;
    }
    return status;
}
