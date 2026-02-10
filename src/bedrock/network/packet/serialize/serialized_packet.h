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

enum class SideBySideValidation : int {
    LogOnError = 0,
    AssertOnError = 1,
};

template <typename PacketInfo_T, typename PacketPayload_T>
class SerializedPayloadPacket : public Packet {
public:
    SerializedPayloadPacket();
    template <typename... ConstructionArgsT>
    SerializedPayloadPacket(ConstructionArgsT &&...args);
    SerializedPayloadPacket(PacketPayload_T);

    [[nodiscard]] virtual MinecraftPacketIds getId() const
    {
        return PacketInfo_T::PACKET_ID;
    }
    [[nodiscard]] virtual std::string_view getName() const
    {
        return PacketInfo_T::PACKET_NAME;
    }
    [[nodiscard]] virtual SerializationMode getSerializationMode() const
    {
        return serialization_mode;
    }
    virtual void setSerializationMode(SerializationMode mode)
    {
        serialization_mode = mode;
    }
    virtual void writeWithSerializationMode(BinaryStream &, const cereal::ReflectionCtx &,
                                            std::optional<SerializationMode>) const;
    virtual void write(BinaryStream &, const cereal::ReflectionCtx &) const;
    virtual void write(BinaryStream &) const;
    virtual Bedrock::Result<void> read(ReadOnlyBinaryStream &, const cereal::ReflectionCtx &);
    virtual Bedrock::Result<void> read(ReadOnlyBinaryStream &);

private:
    virtual Bedrock::Result<void> _read(ReadOnlyBinaryStream &, const cereal::ReflectionCtx &);
    virtual Bedrock::Result<void> _read(ReadOnlyBinaryStream &);

    void setPayload(PacketPayload_T);
    void manualWrite(BinaryStream &, const PacketPayload_T &) const;
    Bedrock::Result<void> manualRead(ReadOnlyBinaryStream &, PacketPayload_T &) const;
    void cerealWrite(BinaryStream &, const cereal::ReflectionCtx &, const PacketPayload_T &) const;
    Bedrock::Result<void> cerealRead(ReadOnlyBinaryStream &, const cereal::ReflectionCtx &, PacketPayload_T &) const;
    void sideBySideWrite(BinaryStream &, const cereal::ReflectionCtx &, const PacketPayload_T &,
                         const SideBySideValidation) const;
    void semanticSideBySideWrite(BinaryStream &, const cereal::ReflectionCtx &, const PacketPayload_T &,
                                 const SideBySideValidation) const;
    Bedrock::Result<void> sideBySideRead(ReadOnlyBinaryStream &, const cereal::ReflectionCtx &, PacketPayload_T &,
                                         const SideBySideValidation);
    Bedrock::Result<void> semanticSideBySideRead(ReadOnlyBinaryStream &, const cereal::ReflectionCtx &,
                                                 PacketPayload_T &, const SideBySideValidation);

public:
    PacketPayload_T payload;
    SerializationMode serialization_mode;
};
