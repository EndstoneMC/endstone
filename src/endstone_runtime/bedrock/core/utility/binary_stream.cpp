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

#include "bedrock/core/utility/binary_stream.h"

#include <fmt/core.h>

void BinaryStream::write(const void *data, std::size_t size)
{
    if (size > 0) {
        buffer_->append(static_cast<const char *>(data), size);
    }
}

void BinaryStream::writeUnsignedChar(std::uint8_t value)
{
    write(&value, sizeof(std::uint8_t));
}

void BinaryStream::writeByte(std::uint8_t value)
{
    write(&value, sizeof(std::uint8_t));
}

void BinaryStream::writeBool(bool value)
{
    writeByte(value ? 1 : 0);
}

void BinaryStream::writeVarInt(std::int32_t value)
{
    writeUnsignedVarInt((value >> 31) ^ (value << 1));
}

void BinaryStream::writeVarInt64(std::int64_t value)
{
    writeUnsignedVarInt64((value >> 63) ^ (value << 1));
}

void BinaryStream::writeUnsignedVarInt(std::uint32_t value)
{
    do {
        std::uint8_t byte = value & 0xFF;
        value >>= 7;
        if (value) {
            writeByte(byte | 0x80);
        }
        else {
            writeByte(byte & 0x7F);
        }
    } while (value);
}

void BinaryStream::writeUnsignedVarInt64(std::uint64_t value)
{
    do {
        std::uint8_t byte = value & 0xFF;
        value >>= 7;
        if (value) {
            writeByte(byte | 0x80);
        }
        else {
            writeByte(byte & 0x7F);
        }
    } while (value);
}

void BinaryStream::writeString(std::string_view value)
{
    writeUnsignedVarInt(value.length());
    write(value.data(), value.size());
}

void BinaryStream::writeFloat(float value)
{
    write(&value, sizeof(float));
}
