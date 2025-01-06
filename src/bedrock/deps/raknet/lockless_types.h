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

#ifdef _WIN32
#include <Windows.h>
#endif

#include <cstdint>

namespace RakNet {

class LocklessUint32_t {  // NOLINT
public:
    explicit LocklessUint32_t(std::uint32_t initial = 0) : value_(initial){};

    std::uint32_t Increment()  // NOLINT
    {
#ifdef _WIN32
        return static_cast<uint32_t>(InterlockedIncrement(&value_));
#else
        return __sync_fetch_and_add(&value_, static_cast<std::uint32_t>(1));
#endif
    }

    std::uint32_t Decrement()  // NOLINT
    {
#ifdef _WIN32
        return static_cast<uint32_t>(InterlockedDecrement(&value_));
#else
        return __sync_fetch_and_add(&value_, static_cast<std::uint32_t>(-1));
#endif
    }

    [[nodiscard]] std::uint32_t GetValue() const  // NOLINT
    {
        return value_;
    }

protected:
    volatile uint32_t value_{0};
};

}  // namespace RakNet
