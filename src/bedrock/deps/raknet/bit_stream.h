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

#include <algorithm>
#include <bit>

#include "bedrock/deps/raknet/raknet_types.h"
#include "raknet_defines.h"

namespace RakNet {
class BitStream {
public:
    BitStream();
    BitStream(unsigned char *_data, unsigned int lengthInBytes, bool _copyData);

    void IgnoreBits(BitSize_t numberOfBits) { readOffset += numberOfBits; }
    void IgnoreBytes(unsigned int numberOfBytes) { IgnoreBits(BYTES_TO_BITS(numberOfBytes)); }

    template <class templateType>
    bool Read(templateType &outTemplateVar)
    {
        if (sizeof(outTemplateVar) == 1) {
            return ReadBits(reinterpret_cast<unsigned char *>(&outTemplateVar), sizeof(templateType) * 8, true);
        }

        if (DoEndianSwap()) {
            unsigned char output[sizeof(templateType)];
            if (ReadBits(static_cast<unsigned char *>(output), sizeof(templateType) * 8, true)) {
                ReverseBytes(output, reinterpret_cast<unsigned char *>(&outTemplateVar), sizeof(templateType));
                return true;
            }
            return false;
        }
        else {
            return ReadBits(reinterpret_cast<unsigned char *>(&outTemplateVar), sizeof(templateType) * 8, true);
        }
    }

    void ReverseBytes(unsigned char *inByteArray, unsigned char *inOutByteArray, unsigned int length);

    template <>
    bool Read(RakNetGUID &outTemplateVar)
    {
        return Read(outTemplateVar.g);
    }

    bool ReadBits(unsigned char *inOutByteArray, BitSize_t numberOfBitsToRead, bool alignBitsToRight);

    template <class templateType>
    void Write(const templateType &inTemplateVar)
    {
        if (sizeof(inTemplateVar) == 1) {
            WriteBits((unsigned char *)&inTemplateVar, sizeof(templateType) * 8, true);
        }
        else {
            if (DoEndianSwap()) {
                unsigned char output[sizeof(templateType)];
                ReverseBytes((unsigned char *)&inTemplateVar, output, sizeof(templateType));
                WriteBits(reinterpret_cast<unsigned char *>(output), sizeof(templateType) * 8, true);
            }
            else {
                WriteBits((unsigned char *)&inTemplateVar, sizeof(templateType) * 8, true);
            }
        }
    }

    template <>
    void Write(const RakNetGUID &inTemplateVar)
    {
        Write(inTemplateVar.g);
    }

    void Write(const char *inputByteArray, unsigned int numberOfBytes);

    void AlignWriteToByteBoundary() { numberOfBitsUsed += 8 - (((numberOfBitsUsed - 1) & 7) + 1); }

    void WriteAlignedBytes(const unsigned char *inByteArray, unsigned int numberOfBytesToWrite)
    {
        AlignWriteToByteBoundary();
        Write(reinterpret_cast<const char *>(inByteArray), numberOfBytesToWrite);
    }

    void WriteBits(const unsigned char *inByteArray, BitSize_t numberOfBitsToWrite, bool rightAlignedBits);
    void AddBitsAndReallocate(BitSize_t numberOfBitsToWrite);

    [[nodiscard]] BitSize_t GetNumberOfBytesUsed() const { return BITS_TO_BYTES(numberOfBitsUsed); }
    [[nodiscard]] unsigned char *GetData() const { return data; }

    static bool DoEndianSwap() { return !IsNetworkOrderInternal(); }
    static constexpr bool IsNetworkOrderInternal() { return std::endian::native == std::endian::big; }

private:
    BitSize_t numberOfBitsUsed;
    BitSize_t numberOfBitsAllocated;
    BitSize_t readOffset;
    unsigned char *data;
    bool copyData;
    unsigned char stackData[BITSTREAM_STACK_ALLOCATION_SIZE];
};
}  // namespace RakNet
