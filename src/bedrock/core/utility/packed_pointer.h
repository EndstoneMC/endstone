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
#include <entt/entity/storage.hpp>

namespace Bedrock::PackedPointer {

using StorageType = std::uint64_t;
using PackedFieldType = std::uint8_t;
using CombinedFieldType = std::uint16_t;

namespace Detail {
template <size_t AddressAlignmentBits>
struct Packed64Config {
    // Basic constants
    static constexpr size_t ADDRESS_BITS = 48;
    static constexpr size_t ADDRESS_ALIGNMENT_BITS = AddressAlignmentBits;
    static constexpr size_t TAG_BITS = 8;
    static constexpr int FIELD8_SHIFT = ADDRESS_BITS;
    static constexpr int FLAGS_SHIFT = 0;  // flags are in the lowest bits

    // Masks
    static constexpr uint64_t TAG_MASK = 0xFFULL << FIELD8_SHIFT << TAG_BITS;  // bits 56..63
    static constexpr uint64_t FIELD8_MASK = 0xFFULL << FIELD8_SHIFT;           // bits 48..55

    // Flags take up the low 'AlignmentBits' bits，e.g. 0x1 for 1-bit, 0x3 for 2-bits, etc.
    static constexpr uint64_t FLAGS_MASK = (1ULL << ADDRESS_ALIGNMENT_BITS) - 1ULL;

    // Address is in bits [AlignmentBits .. 47]
    static constexpr uint64_t ADDRESS_MASK =
        ((1ULL << ADDRESS_BITS) - 1ULL) & ~FLAGS_MASK;  // zero out the low flag bits
    static constexpr uint64_t POINTER_MASK = TAG_MASK | ADDRESS_MASK;
    static constexpr uint64_t FLAGS_MAX_VALUE = FLAGS_MASK;
};
static_assert(Packed64Config<2UL>::FLAGS_MASK == 3);
static_assert(Packed64Config<2UL>::POINTER_MASK == 0xFF00FFFFFFFFFFFC);
}  // namespace Detail

template <typename T, bool VALIDATE, size_t ADDRESS64_ALIGNMENT_BITS>
class Packed {
private:
    using Config = Detail::Packed64Config<ADDRESS64_ALIGNMENT_BITS>;

public:
    Packed(StorageType storage) : storage_(storage) {}

    [[nodiscard]] StorageType getStorage() const
    {
        return storage_;
    }

    [[nodiscard]] T *getPointer() const
    {
        return reinterpret_cast<T *>(storage_ & Config::POINTER_MASK);
    }

    [[nodiscard]] PackedFieldType getData8() const
    {
        return static_cast<PackedFieldType>((storage_ & Config::FIELD8_MASK) >> Config::FIELD8_SHIFT);
    }

    void setPointer(const char *pointer)
    {
        storage_ &= ~Config::ADDRESS_MASK;
        storage_ |= reinterpret_cast<uintptr_t>(pointer) & Config::ADDRESS_MASK;
    }

    void setData8(PackedFieldType field8)
    {
        storage_ &= ~Config::FIELD8_MASK;
        storage_ |= (static_cast<StorageType>(field8) << Config::FIELD8_SHIFT) & Config::FIELD8_MASK;
    }

    void reset()
    {
        storage_ = 0;
    }

private:
    StorageType storage_;  // +0
};

template <typename T>
using Packed1ByteAligned = Packed<T, true, 0UL>;

}  // namespace Bedrock::PackedPointer
