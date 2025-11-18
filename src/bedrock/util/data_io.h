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

#include <cstdint>
#include <string>

#include "bedrock/platform/result.h"

class IDataOutput {
public:
    virtual ~IDataOutput() = default;
    virtual void writeString(std::string_view v) = 0;
    virtual void writeLongString(std::string_view v) = 0;
    virtual void writeFloat(float v) = 0;
    virtual void writeDouble(double v) = 0;
    virtual void writeByte(std::uint8_t v) = 0;
    virtual void writeShort(std::int16_t v) = 0;
    virtual void writeInt(std::int32_t v) = 0;
    virtual void writeLongLong(std::int64_t v) = 0;
    virtual void writeBytes(void const *data, std::size_t bytes) = 0;
};

class IDataInput {
public:
    virtual ~IDataInput() = default;
    virtual Bedrock::Result<std::string> readStringResult() = 0;
    virtual Bedrock::Result<std::string> readLongStringResult() = 0;
    virtual Bedrock::Result<float> readFloatResult() = 0;
    virtual Bedrock::Result<double> readDoubleResult() = 0;
    virtual Bedrock::Result<std::uint8_t> readByteResult() = 0;
    virtual Bedrock::Result<std::int16_t> readShortResult() = 0;
    virtual Bedrock::Result<std::int32_t> readIntResult() = 0;
    virtual Bedrock::Result<std::int64_t> readLongLongResult() = 0;
    virtual Bedrock::Result<void> readBytesResult(void *, std::uint64_t) = 0;
    [[nodiscard]] virtual std::size_t numBytesLeft() const = 0;
};

class BytesDataOutput : public IDataOutput {
public:
    ~BytesDataOutput() override = default;
    void writeString(std::string_view v) override;
    void writeLongString(std::string_view v) override;
    void writeFloat(float v) override;
    void writeDouble(double v) override;
    void writeByte(std::uint8_t v) override;
    void writeShort(std::int16_t v) override;
    void writeInt(std::int32_t v) override;
    void writeLongLong(std::int64_t v) override;
    void writeBytes(const void *, size_t) override = 0;
};

class BytesDataInput : public IDataInput {
public:
    Bedrock::Result<std::string> readStringResult() override;
     Bedrock::Result<std::string> readLongStringResult() override;
     Bedrock::Result<float> readFloatResult() override;
     Bedrock::Result<double> readDoubleResult() override;
     Bedrock::Result<std::uint8_t> readByteResult() override;
     Bedrock::Result<std::int16_t> readShortResult() override;
     Bedrock::Result<std::int32_t> readIntResult() override;
     Bedrock::Result<std::int64_t> readLongLongResult() override;
     Bedrock::Result<void> readBytesResult(void *, size_t) override = 0;
};

class PrintStream {
public:
    virtual ~PrintStream() = 0;
    virtual void print(std::string const &) = 0;
};
