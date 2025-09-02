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

#include <string>

#include "bedrock/network/packet.h"

struct ModalFormRequestPacketPayload {
    ModalFormRequestPacketPayload();
    ModalFormRequestPacketPayload(uint32_t, const std::string &);

    std::uint32_t form_id;
    std::string form_json;
};

class ModalFormRequestPacket : public Packet {
public:
    static constexpr bool SHARE_WITH_HANDLER = false;

    ModalFormRequestPacket();
    ModalFormRequestPacket(ModalFormRequestPacketPayload);

    [[nodiscard]] MinecraftPacketIds getId() const override;
    [[nodiscard]] std::string getName() const override;
    [[nodiscard]] SerializationMode getSerializationMode() const override;
    void setSerializationMode(SerializationMode) override;
    void writeWithSerializationMode(BinaryStream &, const cereal::ReflectionCtx &,
                                    std::optional<SerializationMode>) const override;
    void write(BinaryStream &, const cereal::ReflectionCtx &) const override;
    Bedrock::Result<void> read(ReadOnlyBinaryStream &, const cereal::ReflectionCtx &) override;
    void write(BinaryStream &) const override;
    Bedrock::Result<void> read(ReadOnlyBinaryStream &) override;

private:
    Bedrock::Result<void> _read(ReadOnlyBinaryStream &) override;
    Bedrock::Result<void> _read(ReadOnlyBinaryStream &, const cereal::ReflectionCtx &) override;

public:
    ModalFormRequestPacketPayload payload;
    SerializationMode serialization_mode;
};
