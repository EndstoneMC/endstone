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

#include <entt/core/hashed_string.hpp>

#include "bedrock/platform/result.h"

class ReadOnlyBinaryStream {
public:
    explicit ReadOnlyBinaryStream(std::string_view buffer, bool copy_buffer);
    virtual ~ReadOnlyBinaryStream() = default;

private:
    virtual Bedrock::Result<void> read(void *target, std::uint64_t num);

public:
    void setReadPointer(size_t);
    [[nodiscard]] size_t getReadPointer() const;
    [[nodiscard]] size_t getUnreadLength() const;
    [[nodiscard]] size_t getLength() const;
    Bedrock::Result<unsigned char> getByte();
    Bedrock::Result<unsigned int> getUnsignedVarInt();
    [[nodiscard]] std::string_view getView() const;
    [[nodiscard]] bool hasOverflowed() const;

protected:
    std::string owned_buffer_;  // +8
    std::string_view view_;     // +40

private:
    std::size_t read_pointer_{0};  // +56
    bool has_overflowed_{false};   // +64
};
BEDROCK_STATIC_ASSERT_SIZE(ReadOnlyBinaryStream, 72, 64);

class BinaryStream : public ReadOnlyBinaryStream {
public:
    using ReadOnlyBinaryStream::ReadOnlyBinaryStream;
    BinaryStream();
    [[nodiscard]] const std::string &getBuffer() const;
    void reset();

    void writeBool(bool value);
    void writeByte(std::uint8_t value);
    void writeUnsignedShort(std::uint16_t value);
    void writeSignedShort(std::int16_t value);
    void writeUnsignedVarInt(std::uint32_t value);
    void writeUnsignedVarInt64(std::uint64_t value);
    void writeVarInt(std::int32_t value);
    void writeVarInt64(std::int64_t value);
    void writeFloat(float value);
    void writeString(std::string_view value);
    void writeRawBytes(std::string_view span);
    void writeStream(BinaryStream &);

private:
    void write(const void *data, std::size_t size);
    std::string *buffer_;  // +72
};
BEDROCK_STATIC_ASSERT_SIZE(BinaryStream, 80, 72);
