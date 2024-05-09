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

class IDataInput {
public:
    virtual ~IDataInput() = default;
    virtual class Bedrock::Result<std::string> readStringResult() = 0;
    virtual class Bedrock::Result<std::string> readLongStringResult() = 0;
    virtual class Bedrock::Result<float> readFloatResult() = 0;
    virtual class Bedrock::Result<double> readDoubleResult() = 0;
    virtual class Bedrock::Result<char> readByteResult() = 0;
    virtual class Bedrock::Result<short> readShortResult() = 0;
    virtual class Bedrock::Result<int> readIntResult() = 0;
    virtual class Bedrock::Result<std::int64_t> readLongLongResult() = 0;
    virtual class Bedrock::Result<void> readBytesResult(void *, std::uint64_t) = 0;
    virtual std::uint64_t numBytesLeft() const = 0;
};
