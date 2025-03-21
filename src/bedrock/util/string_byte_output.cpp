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

#include "bedrock/util/string_byte_output.h"

void StringByteOutput::writeBytes(const void *data, std::uint64_t bytes)
{
    buffer_.append(static_cast<const char *>(data), bytes);
}

void BigEndianStringByteOutput::writeFloat(float v)
{
    auto p = reinterpret_cast<std::uint32_t &>(v);
    writeByte((p >> 24) & 0xff);
    writeByte((p >> 16) & 0xff);
    writeByte((p >> 8) & 0xff);
    writeByte(p & 0xff);
}

void BigEndianStringByteOutput::writeDouble(double v)
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

void BigEndianStringByteOutput::writeShort(std::int16_t v)
{
    writeByte((v >> 8) & 0xff);
    writeByte(v & 0xff);
}

void BigEndianStringByteOutput::writeInt(std::int32_t v)
{
    writeByte((v >> 24) & 0xff);
    writeByte((v >> 16) & 0xff);
    writeByte((v >> 8) & 0xff);
    writeByte(v & 0xff);
}

void BigEndianStringByteOutput::writeLongLong(std::int64_t v)
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
