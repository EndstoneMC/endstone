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

#include "bedrock/core/utility/binary_stream.h"
#include "bedrock/network/packet.h"

namespace endstone::core {

class DataPacket : public Packet {
public:
    DataPacket(int packet_id, std::string_view payload);
    [[nodiscard]] MinecraftPacketIds getId() const override;
    [[nodiscard]] std::string_view getName() const override;
    void write(BinaryStream &stream) const override;
    [[nodiscard]] int getPacketId() const;
    [[nodiscard]] std::string_view getPayload() const;

private:
    [[nodiscard]] Bedrock::Result<void> _read(ReadOnlyBinaryStream &stream) override;

private:
    int packet_id_;
    std::string_view payload_;
};

}  // namespace endstone::core
