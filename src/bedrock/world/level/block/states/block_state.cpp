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

#include "bedrock/world/level/block/states/block_state.h"

size_t BlockState::getID() const
{
    return id_;
}

const HashedString &BlockState::getName() const
{
    return name_;
}

uint32_t BlockStateInstance::getMask() const
{
    return mask_;
}

std::uint32_t BlockStateInstance::getNumBits() const
{
    return num_bits_;
}

bool BlockStateInstance::isValidData(std::uint32_t id) const
{
    return (mask_ & id) >> (end_bit_ - num_bits_ + 1) < variation_count_;
}

const BlockState &BlockStateInstance::getState() const
{
    return *state_;
}
