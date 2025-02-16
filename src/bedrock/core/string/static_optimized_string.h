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

#include <cstdlib>

#include "bedrock/core/utility/packed_pointer.h"

namespace Bedrock {

namespace Memory {
inline void *alignedAllocate(std::size_t size, std::size_t alignment)
{
#if defined(_MSC_VER)
    // On MSVC: void* _aligned_malloc(size_t size, size_t alignment)
    // No rounding up required
    return _aligned_malloc(size, alignment);
#else
    // On non-MSVC platforms (C++17+): void* std::aligned_alloc(size_t alignment, size_t size)
    // Must ensure "size" is a multiple of "alignment"
    std::size_t remainder = size % alignment;
    if (remainder != 0) {
        size += (alignment - remainder);
    }
    return std::aligned_alloc(alignment, size);
#endif
}

inline void alignedRelease(void *ptr)
{
#if defined(_MSC_VER)
    // On MSVC
    _aligned_free(ptr);
#else
    // On other platforms
    std::free(ptr);
#endif
}

}  // namespace Memory

class StaticOptimizedString {
public:
    enum class StorageType : int {
        Static = 0,
        Dynamic = 1,
    };
    static constexpr int MAX_STATIC_STRING_LENGTH = 1 << 7;

    StaticOptimizedString() = default;
    ~StaticOptimizedString()
    {
        if (data_.getData8() == MAX_STATIC_STRING_LENGTH) {
            if (const auto *data_ptr = data_.getPointer()) {
                const auto *size_ptr = reinterpret_cast<const std::size_t *>(data_ptr) - 1;
                void *pointer = const_cast<void *>(static_cast<const void *>(size_ptr));
                Memory::alignedRelease(pointer);
            }
        }
        reset();
    }

    template <std::size_t N>
    StaticOptimizedString(const char (&str)[N])
    {
        _set(str, N - 1, (N - 1) >= MAX_STATIC_STRING_LENGTH ? StorageType::Dynamic : StorageType::Static);
    }

    StaticOptimizedString(std::string_view sv, StorageType storage_type)
    {
        _set(sv.data(), sv.length(), storage_type);
    }

    StaticOptimizedString(const StaticOptimizedString &other)
    {
        _set(other.get(), other.length(),
             other.length() >= MAX_STATIC_STRING_LENGTH ? StorageType::Dynamic : StorageType::Static);
    }

    StaticOptimizedString &operator=(const StaticOptimizedString &other)
    {
        if (this != &other) {
            _set(other.get(), other.length(),
                 other.length() >= MAX_STATIC_STRING_LENGTH ? StorageType::Dynamic : StorageType::Static);
        }
        return *this;
    }

    StaticOptimizedString(StaticOptimizedString &&other) noexcept
    {
        data_ = other.data_;
        other.data_.reset();
    }

    StaticOptimizedString &operator=(StaticOptimizedString &&other) noexcept
    {
        if (this != &other) {
            data_ = other.data_;
            other.data_.reset();
        }
        return *this;
    }

    [[nodiscard]] const char *get() const
    {
        return data_.getPointer();
    }

    [[nodiscard]] std::string_view view() const
    {
        return {get(), length()};
    }

    operator const char *() const
    {
        return get();
    }

    operator std::string_view() const
    {
        return view();
    }

    [[nodiscard]] size_t length() const
    {
        if (data_.getData8() == MAX_STATIC_STRING_LENGTH) {
            const auto *data_ptr = data_.getPointer();
            const auto *size_ptr = reinterpret_cast<const std::size_t *>(data_ptr) - 1;
            return *size_ptr;
        }

        return data_.getData8();
    }

    void reset()
    {
        data_.reset();
    }

private:
    void _set(const char *data, size_t length, StorageType storage_type)
    {
        if (data && (storage_type == StorageType::Dynamic || length >= MAX_STATIC_STRING_LENGTH)) {
            const auto size = length ? length : 1;
            auto *pointer = Memory::alignedAllocate(size + 9, alignof(std::size_t));

            auto *size_ptr = static_cast<std::size_t *>(pointer);
            *size_ptr = length;

            auto *data_ptr = reinterpret_cast<char *>(size_ptr + 1);
            std::memcpy(data_ptr, data, length);
            data_ptr[length] = '\0';

            data_.setPointer(data_ptr);
            data_.setData8(MAX_STATIC_STRING_LENGTH);
        }
        else {
            data_.setPointer(data);
            data_.setData8(length & (MAX_STATIC_STRING_LENGTH - 1));
        }
    }

    PackedPointer::Packed1ByteAligned<const char> data_{0};  // +0
};
static_assert(sizeof(StaticOptimizedString) == 8);
}  // namespace Bedrock
