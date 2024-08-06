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

#include "bedrock/network/packet.h"
#include "endstone/network/packet.h"

namespace endstone::detail {

class PacketAdapter : public ::Packet {
public:
    explicit PacketAdapter(endstone::Packet &packet);

    ~PacketAdapter() override = default;
    [[nodiscard]] virtual MinecraftPacketIds getId() const;
    [[nodiscard]] virtual std::string getName() const;
    [[nodiscard]] virtual Bedrock::Result<void> checkSize(std::uint64_t, bool) const;
    virtual void write(BinaryStream &) const;
    [[nodiscard]] virtual Bedrock::Result<void> read(ReadOnlyBinaryStream &);
    [[nodiscard]] virtual bool disallowBatching() const;
    [[nodiscard]] virtual bool isValid() const;

private:
    [[nodiscard]] virtual Bedrock::Result<void> _read(ReadOnlyBinaryStream &);

    endstone::Packet &packet_;
};

}  // namespace endstone::detail
