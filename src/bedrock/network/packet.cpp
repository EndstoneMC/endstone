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

#include "bedrock/network/packet.h"

#include "bedrock/network/net_event_callback.h"
#include "bedrock/symbol.h"

Bedrock::Result<void> Packet::checkSize(std::uint64_t packet_size, bool is_receiver_server) const
{
    if (is_receiver_server && packet_size > 0xA00000) {
        return BEDROCK_NEW_ERROR(std::errc::message_size);
    }
    return {};
}

void Packet::writeWithSerializationMode(BinaryStream &bit_stream, const cereal::ReflectionCtx &reflection_ctx,
                                        std::optional<SerializationMode> mode) const
{
    write(bit_stream, reflection_ctx);
}

void Packet::write(BinaryStream &bit_stream, const cereal::ReflectionCtx &reflection_ctx) const
{
    write(bit_stream);
}

Bedrock::Result<void> Packet::read(ReadOnlyBinaryStream &stream, const cereal::ReflectionCtx &)
{
    return read(stream);
}

Bedrock::Result<void> Packet::read(ReadOnlyBinaryStream &stream)
{
    if (auto result = _read(stream); !result.ignoreError()) {
        return BEDROCK_RETHROW(result);
    }
    return {};
}

bool Packet::disallowBatching() const
{
    return false;
}

bool Packet::isValid() const
{
    return true;
}

SerializationMode Packet::getSerializationMode() const
{
    return SerializationMode::ManualOnly;
}

void Packet::setSerializationMode(SerializationMode) {}

Bedrock::Result<void> Packet::readNoHeader(ReadOnlyBinaryStream &stream, const cereal::ReflectionCtx &reflection_ctx,
                                           const SubClientId &sub_id)
{
    client_sub_id_ = sub_id;
    auto result = read(stream, reflection_ctx);
    if (!result.ignoreError()) {
        return BEDROCK_RETHROW(result);
    }
    return result;
}

SubClientId Packet::getClientSubId() const
{
    return client_sub_id_;
}

Compressibility Packet::getCompressible() const
{
    return compressible_;
}

NetworkPeer::Reliability Packet::getReliability() const
{
    return reliability_;
}

void Packet::setReceiveTimestamp(const NetworkPeer::PacketRecvTimepoint &recv_timepoint)
{
    recv_timepoint_ = recv_timepoint;
}

void Packet::handle(const NetworkIdentifier &id, NetEventCallback &callback, std::shared_ptr<Packet> &packet)
{
    if (handler_) {
        handler_->handle(id, callback, packet);
    }
}

Bedrock::Result<void> Packet::_read(ReadOnlyBinaryStream &bit_stream, const cereal::ReflectionCtx &reflection_ctx)
{
    return _read(bit_stream);
}
