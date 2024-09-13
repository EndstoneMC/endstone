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

#include "bedrock/util/data_io.h"

void BytesDataOutput::writeString(std::string_view v)
{
    writeShort(static_cast<std::int16_t>(v.size() & 0x7fff));
    writeBytes(v.data(), v.size() & 0x7fff);
}

void BytesDataOutput::writeLongString(std::string_view v)
{
    writeInt(static_cast<std::int32_t>(v.size()));
    writeBytes(v.data(), v.size());
}

void BytesDataOutput::writeFloat(float v)
{
    writeBytes(&v, sizeof(float));
}

void BytesDataOutput::writeDouble(double v)
{
    writeBytes(&v, sizeof(double));
}

void BytesDataOutput::writeByte(std::uint8_t v)
{
    writeBytes(&v, sizeof(std::uint8_t));
}

void BytesDataOutput::writeShort(std::int16_t v)
{
    writeBytes(&v, sizeof(std::int16_t));
}

void BytesDataOutput::writeInt(std::int32_t v)
{
    writeBytes(&v, sizeof(std::int32_t));
}

void BytesDataOutput::writeLongLong(std::int64_t v)
{
    writeBytes(&v, sizeof(std::int64_t));
}
