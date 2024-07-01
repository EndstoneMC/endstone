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

#include "bedrock/common/util/data_io.h"

class BytesDataOutput : public IDataOutput {

public:
    ~BytesDataOutput() override = default;
    void writeString(std::string_view v) override
    {
        writeShort(static_cast<std::int16_t>(v.size() & 0x7fff));
        writeBytes(v.data(), v.size() & 0x7fff);
    }

    void writeLongString(std::string_view v) override
    {
        writeInt(static_cast<std::int32_t>(v.size()));
        writeBytes(v.data(), v.size());
    }

    void writeFloat(float v) override
    {
        writeBytes(&v, sizeof(float));
    }

    void writeDouble(double v) override
    {
        writeBytes(&v, sizeof(double));
    }

    void writeByte(std::uint8_t v) override
    {
        writeBytes(&v, sizeof(std::uint8_t));
    }

    void writeShort(std::int16_t v) override
    {
        writeBytes(&v, sizeof(std::int16_t));
    }

    void writeInt(std::int32_t v) override
    {
        writeBytes(&v, sizeof(std::int32_t));
    }

    void writeLongLong(std::int64_t v) override
    {
        writeBytes(&v, sizeof(std::int64_t));
    }
};

class StringByteOutput : public BytesDataOutput {
public:
    ~StringByteOutput() override = default;
    void writeBytes(const void *data, std::uint64_t bytes) override
    {
        buffer.append(static_cast<const char *>(data), bytes);
    }
    std::string buffer;
};

class BigEndianStringByteOutput : public StringByteOutput {
public:
    ~BigEndianStringByteOutput() override = default;
    void writeFloat(float v) override
    {
        auto p = reinterpret_cast<std::uint32_t &>(v);
        writeByte((p >> 24) & 0xff);
        writeByte((p >> 16) & 0xff);
        writeByte((p >> 8) & 0xff);
        writeByte(p & 0xff);
    }

    void writeDouble(double v) override
    {
        auto p = reinterpret_cast<std::uint64_t &>(v);
        writeByte((p >> 56) & 0xff);
        writeByte((p >> 48) & 0xff);
        writeByte((p >> 40) & 0xff);
        writeByte((p >> 32) & 0xff);
        writeByte((p >> 24) & 0xff);
        writeByte((p >> 16) & 0xff);
        writeByte((p >> 8) & 0xff);
        writeByte(p & 0xff);
    }

    void writeShort(std::int16_t v) override
    {
        writeByte((v >> 8) & 0xff);
        writeByte(v & 0xff);
    }

    void writeInt(std::int32_t v) override
    {
        writeByte((v >> 24) & 0xff);
        writeByte((v >> 16) & 0xff);
        writeByte((v >> 8) & 0xff);
        writeByte(v & 0xff);
    }

    void writeLongLong(std::int64_t v) override
    {
        writeByte((v >> 56) & 0xff);
        writeByte((v >> 48) & 0xff);
        writeByte((v >> 40) & 0xff);
        writeByte((v >> 32) & 0xff);
        writeByte((v >> 24) & 0xff);
        writeByte((v >> 16) & 0xff);
        writeByte((v >> 8) & 0xff);
        writeByte(v & 0xff);
    }
};
