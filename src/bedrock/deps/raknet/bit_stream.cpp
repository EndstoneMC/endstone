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

#include "bedrock/deps/raknet/bit_stream.h"

#include <algorithm>
#include <cstring>



namespace RakNet {
BitStream::BitStream()
{
    numberOfBitsUsed = 0;
    numberOfBitsAllocated = BITSTREAM_STACK_ALLOCATION_SIZE * 8;
    readOffset = 0;
    data = static_cast<unsigned char *>(stackData);
    copyData = true;
}

BitStream::BitStream(unsigned char *_data, unsigned int lengthInBytes, bool _copyData)
{
    numberOfBitsUsed = lengthInBytes << 3;
    readOffset = 0;
    copyData = _copyData;
    numberOfBitsAllocated = lengthInBytes << 3;
    if (copyData) {
        if (lengthInBytes > 0) {
            if (lengthInBytes < BITSTREAM_STACK_ALLOCATION_SIZE) {
                data = static_cast<unsigned char *>(stackData);
                numberOfBitsAllocated = BITSTREAM_STACK_ALLOCATION_SIZE << 3;
            }
            else {
                data = static_cast<unsigned char *>(malloc(lengthInBytes));
            }
            memcpy(data, _data, lengthInBytes);
        }
        else {
            data = nullptr;
        }
    }
    else {
        data = _data;
    }
}

void BitStream::ReverseBytes(unsigned char *inByteArray, unsigned char *inOutByteArray, const unsigned int length)
{
    for (BitSize_t i = 0; i < length; i++) {
        inOutByteArray[i] = inByteArray[length - i - 1];
    }
}

bool BitStream::ReadBits(unsigned char *inOutByteArray, BitSize_t numberOfBitsToRead, const bool alignBitsToRight)
{
    if (numberOfBitsToRead <= 0) {
        return false;
    }

    if (readOffset + numberOfBitsToRead > numberOfBitsUsed) {
        return false;
    }

    const BitSize_t readOffsetMod8 = readOffset & 7;
    if (readOffsetMod8 == 0 && (numberOfBitsToRead & 7) == 0) {
        memcpy(inOutByteArray, data + (readOffset >> 3), numberOfBitsToRead >> 3);
        readOffset += numberOfBitsToRead;
        return true;
    }

    BitSize_t offset = 0;
    memset(inOutByteArray, 0, BITS_TO_BYTES(numberOfBitsToRead));
    while (numberOfBitsToRead > 0) {
        *(inOutByteArray + offset) |= *(data + (readOffset >> 3)) << (readOffsetMod8);

        if (readOffsetMod8 > 0 && numberOfBitsToRead > 8 - (readOffsetMod8)) {
            *(inOutByteArray + offset) |= *(data + (readOffset >> 3) + 1) >> (8 - (readOffsetMod8));
        }

        if (numberOfBitsToRead >= 8) {
            numberOfBitsToRead -= 8;
            readOffset += 8;
            offset++;
        }
        else {
            int neg = static_cast<int>(numberOfBitsToRead) - 8;
            if (neg < 0) {
                if (alignBitsToRight) {
                    *(inOutByteArray + offset) >>= -neg;
                }
                readOffset += 8 + neg;
            }
            else {
                readOffset += 8;
            }
            offset++;
            numberOfBitsToRead = 0;
        }
    }
    return true;
}

void BitStream::Write(const char *inputByteArray, unsigned int numberOfBytes)
{
    if (numberOfBytes == 0) {
        return;
    }

    if ((numberOfBitsUsed & 7) == 0) {
        AddBitsAndReallocate(BYTES_TO_BITS(numberOfBytes));
        memcpy(data + BITS_TO_BYTES(numberOfBitsUsed), inputByteArray, numberOfBytes);
        numberOfBitsUsed += BYTES_TO_BITS(numberOfBytes);
    }
    else {
        WriteBits((unsigned char *)inputByteArray, numberOfBytes * 8, true);
    }
}

void BitStream::WriteBits(const unsigned char *inByteArray, BitSize_t numberOfBitsToWrite, const bool rightAlignedBits)
{
    AddBitsAndReallocate(numberOfBitsToWrite);
    const BitSize_t numberOfBitsUsedMod8 = numberOfBitsUsed & 7;
    // If currently aligned and numberOfBits is a multiple of 8, just memcpy for speed
    if (numberOfBitsUsedMod8 == 0 && (numberOfBitsToWrite & 7) == 0) {
        memcpy(data + (numberOfBitsUsed >> 3), inByteArray, numberOfBitsToWrite >> 3);
        numberOfBitsUsed += numberOfBitsToWrite;
        return;
    }

    unsigned char dataByte;
    const unsigned char *inputPtr = inByteArray;
    while (numberOfBitsToWrite > 0) {
        dataByte = *(inputPtr++);

        if (numberOfBitsToWrite < 8 && rightAlignedBits) {
            dataByte <<= 8 - numberOfBitsToWrite;
        }

        // Writing to a new byte each time
        if (numberOfBitsUsedMod8 == 0) {
            *(data + (numberOfBitsUsed >> 3)) = dataByte;
        }
        else {
            // Copy over the new data.
            *(data + (numberOfBitsUsed >> 3)) |= dataByte >> (numberOfBitsUsedMod8);

            if (8 - (numberOfBitsUsedMod8) < 8 && 8 - (numberOfBitsUsedMod8) < numberOfBitsToWrite) {
                *(data + (numberOfBitsUsed >> 3) + 1) =
                    static_cast<unsigned char>(dataByte << (8 - (numberOfBitsUsedMod8)));
            }
        }

        if (numberOfBitsToWrite >= 8) {
            numberOfBitsUsed += 8;
            numberOfBitsToWrite -= 8;
        }
        else {
            numberOfBitsUsed += numberOfBitsToWrite;
            numberOfBitsToWrite = 0;
        }
    }
}

void BitStream::AddBitsAndReallocate(const BitSize_t numberOfBitsToWrite)
{
    BitSize_t newNumberOfBitsAllocated = numberOfBitsToWrite + numberOfBitsUsed;
    if (numberOfBitsToWrite + numberOfBitsUsed > 0 &&
        ((numberOfBitsAllocated - 1) >> 3) < ((newNumberOfBitsAllocated - 1) >> 3)) {
        newNumberOfBitsAllocated = (numberOfBitsToWrite + numberOfBitsUsed) * 2;
        if (newNumberOfBitsAllocated - (numberOfBitsToWrite + numberOfBitsUsed) > 1048576) {
            newNumberOfBitsAllocated = numberOfBitsToWrite + numberOfBitsUsed + 1048576;
        }

        BitSize_t amountToAllocate = BITS_TO_BYTES(newNumberOfBitsAllocated);
        if (data == static_cast<unsigned char *>(stackData)) {
            if (amountToAllocate > BITSTREAM_STACK_ALLOCATION_SIZE) {
                data = static_cast<unsigned char *>(malloc(amountToAllocate));
                memcpy(data, stackData, BITS_TO_BYTES(numberOfBitsAllocated));
            }
        }
        else {
            data = static_cast<unsigned char *>(realloc(data, amountToAllocate));
        }
    }
    numberOfBitsAllocated = std::max(newNumberOfBitsAllocated, numberOfBitsAllocated);
}
}  // namespace RakNet
