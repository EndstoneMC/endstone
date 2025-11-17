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

#include "bedrock/util/data_io.h"

class StringByteInput : public BytesDataInput {
public:
    explicit StringByteInput(std::string_view buffer);
    Bedrock::Result<void> readBytesResult(void *, size_t) override;
    size_t numBytesLeft() const override;

protected:
    size_t idx_;
    std::string_view buffer_;
};

class BigEndianStringByteInput : public StringByteInput {
public:
    BigEndianStringByteInput(const std::string &);
    Bedrock::Result<void> readBytesResult(void *, size_t) override;
    virtual Bedrock::Result<void> readBigEndianBytesResult(void *, size_t);
    Bedrock::Result<float> readFloatResult() override;
    Bedrock::Result<double> readDoubleResult() override;
    Bedrock::Result<std::int16_t> readShortResult() override;
    Bedrock::Result<std::int32_t> readIntResult() override;
    Bedrock::Result<std::int64_t> readLongLongResult() override;
};
