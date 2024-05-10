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

class IDataOutput {
public:
    virtual ~IDataOutput() = default;
    virtual void writeString(std::string_view v) = 0;
    virtual void writeLongString(std::string_view v) = 0;
    virtual void writeFloat(float v) = 0;
    virtual void writeDouble(double v) = 0;
    virtual void writeByte(std::int8_t v) = 0;
    virtual void writeShort(std::int16_t v) = 0;
    virtual void writeInt(std::int32_t v) = 0;
    virtual void writeLongLong(std::int64_t v) = 0;
    virtual void writeBytes(void const *data, std::uint64_t bytes) = 0;
};
