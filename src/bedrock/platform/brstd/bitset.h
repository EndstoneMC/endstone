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

#include <array>

#include "bedrock/platform/brstd/detail/bitset_detail.h"

namespace brstd {

template <std::size_t Bits, typename UnderlyingType>
constexpr std::size_t get_num_values()
{
    static_assert(std::is_integral_v<UnderlyingType>, "UnderlyingType must be an integral type.");
    static_assert(Bits > 0, "Bits must be greater than zero.");
    constexpr std::size_t bits_per_type = sizeof(UnderlyingType) * 8;
    return (Bits + bits_per_type - 1) / bits_per_type;
}
static_assert(get_num_values<78, std::uint64_t>() == 2);

template <std::size_t Bits, typename UnderlyingType>
class bitset : public detail::bitset_base<bitset<Bits, UnderlyingType>,
                                          std::array<UnderlyingType, get_num_values<Bits, UnderlyingType>()>> {
    static constexpr size_t num_values = get_num_values<Bits, UnderlyingType>();
    using underlying_type =
        typename detail::bitset_base<bitset, std::array<UnderlyingType, num_values>>::underlying_type;
};
static_assert(sizeof(bitset<2, std::uint8_t>) == 1);
static_assert(sizeof(bitset<11, std::uint16_t>) == 2);
static_assert(sizeof(bitset<17, std::uint32_t>) == 4);
static_assert(sizeof(bitset<78, std::uint64_t>) == 16);

};  // namespace brstd
