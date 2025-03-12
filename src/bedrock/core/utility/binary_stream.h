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
    explicit ReadOnlyBinaryStream(std::string_view buffer, bool copy_buffer)
    {
        auto view = buffer;
        if (copy_buffer) {
            owned_buffer_ = buffer;
            view = owned_buffer_;
        }
        read_pointer_ = 0;
        has_overflowed_ = false;
        view_ = view;
    }
    virtual ~ReadOnlyBinaryStream() = default;

private:
    virtual Bedrock::Result<void> read(void *target, std::uint64_t num);

public:
    [[nodiscard]] size_t getReadPointer() const;
    Bedrock::Result<unsigned char> getByte();
    Bedrock::Result<unsigned int> getUnsignedVarInt();
    [[nodiscard]] std::string_view getView() const;

protected:
    std::string owned_buffer_;  // +8
    std::string_view view_;     // +40

private:
    std::size_t read_pointer_;  // +56
    bool has_overflowed_;       // +64
};
BEDROCK_STATIC_ASSERT_SIZE(ReadOnlyBinaryStream, 72, 64);

class BinaryStream : public ReadOnlyBinaryStream {
public:
    BinaryStream();
    [[nodiscard]] const std::string &getBuffer() const;

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

private:
    void write(const void *data, std::size_t size);
    std::string *buffer_;  // +72
};
BEDROCK_STATIC_ASSERT_SIZE(BinaryStream, 80, 72);
