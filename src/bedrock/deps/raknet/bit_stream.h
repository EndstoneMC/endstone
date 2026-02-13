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

#include "bedrock/deps/raknet/raknet_types.h"

namespace RakNet {
class BitStream {
public:
    BitStream();
    BitStream(unsigned char *_data, unsigned int lengthInBytes, bool _copyData);
    void IgnoreBits(BitSize_t numberOfBits);
    void IgnoreBytes(unsigned int numberOfBytes);
    template <class templateType>
    bool Read(templateType &outTemplateVar);
    template <class templateType>
    void Write(const templateType &inTemplateVar);
    void Write(const char *inputByteArray, unsigned int numberOfBytes);
    void WriteAlignedBytes(const unsigned char *inByteArray, unsigned int numberOfBytesToWrite);
    [[nodiscard]] BitSize_t GetNumberOfBytesUsed() const { return BITS_TO_BYTES(numberOfBitsUsed); }
    [[nodiscard]] unsigned char *GetData() const { return data; }

private:
    BitSize_t numberOfBitsUsed;
    BitSize_t numberOfBitsAllocated;
    BitSize_t readOffset;
    unsigned char *data;
    bool copyData;
    unsigned char stackData[BITSTREAM_STACK_ALLOCATION_SIZE];
};
}  // namespace RakNet
