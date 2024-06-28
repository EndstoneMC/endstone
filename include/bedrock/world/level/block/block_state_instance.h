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

class BlockStateInstance {
public:
private:
    constexpr inline static std::uint32_t max_bits = 16;

    std::uint32_t start_bit_;        // +0
    std::uint32_t num_bits_;         // +4
    std::uint32_t variation_count_;  // +8
    std::uint32_t mask_;             // +12
    bool initialised_;               // +16
    BlockState *state_;              // +24
};
