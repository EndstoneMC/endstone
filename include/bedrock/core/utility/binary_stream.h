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

#include <endstone/network/spawn_particle_effect_packet.h>

#include "bedrock/core/result.h"

class ReadOnlyBinaryStream {
public:
    virtual ~ReadOnlyBinaryStream();
    virtual Bedrock::Result<void> read(void *, std::uint64_t);

private:
    std::size_t read_pointer_;  // +8
    bool has_overflowed_;       // +16
    std::string owned_buffer_;  // +24
    std::string *buffer_;       // +56
};

class BinaryStream : public ReadOnlyBinaryStream {
public:
    void write(const void *data, std::size_t size);
    void writeUnsignedChar(std::uint8_t value);
    void writeByte(std::uint8_t value);
    void writeBool(bool value);
    void writeVarInt(std::int32_t value);
    void writeVarInt64(std::int64_t value);
    void writeUnsignedVarInt(std::uint32_t value);
    void writeUnsignedVarInt64(std::uint64_t value);
    void writeString(std::string_view value);
    void writeFloat(float value);

private:
    std::string owned_buffer_; // +64
    std::string *buffer_; // +96
};
