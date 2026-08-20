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

#include "bedrock/common_types.h"

class Actor;
class BlockPos;
class InteractionResult;
class ItemStack;
class Player;

namespace endstone::runtime {

enum class BucketEmptyAction {
    Continue,
    Cancel,
    Consume,
};

void cancelBucketEmptyResult();
void handleCauldronBucketEmptyResult(::Player &player, const ::BlockPos &position, bool successful);
BucketEmptyAction handleBucketEmptyEvent(::Actor &actor, const ::BlockPos &position, FacingID face,
                                         const ::ItemStack &item_stack);
void handleBucketEmptyResult(const ::InteractionResult &result, ::ItemStack &item_stack, ::Actor &actor,
                             const ::BlockPos &position);

}  // namespace endstone::runtime
