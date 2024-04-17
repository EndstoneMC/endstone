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

#include "bedrock/details.h"
#include "bedrock/world/level/event/coordinator_result.h"

template <std::size_t N>
struct ItemEventPlaceHolder {
    char pad[N];
};

template <typename Return>
struct ItemGameplayEvent;

struct ShapedRecipeTriggeredEvent {};
struct CraftUpdateResultItemClientEvent {};

template <>
struct ItemGameplayEvent<void> {
    std::variant<Details::ValueOrRef<ShapedRecipeTriggeredEvent const>,        // 0
                 Details::ValueOrRef<CraftUpdateResultItemClientEvent const>,  // 1
                 Details::ValueOrRef<ItemEventPlaceHolder<160> const>>         // TODO(fixme): check size on Windows
        event;
};

template <typename Return>
struct MutableItemGameplayEvent;

struct ItemDefinitionEventTriggeredEvent {};
struct ItemUseOnEvent {};
struct ItemUseEvent {};
struct ItemUsedOnEvent {};
struct ItemStartUseOnEvent {};
struct ItemStopUseOnEvent {};
struct ItemStartUseEvent {};
struct ItemCompleteUseEvent {};
struct ItemReleaseUseEvent {};
struct ItemStopUseEvent {};

template <>
struct MutableItemGameplayEvent<CoordinatorResult> {
    std::variant<Details::ValueOrRef<ItemDefinitionEventTriggeredEvent>,  // 0
                 Details::ValueOrRef<ItemUseOnEvent>,                     // 1
                 Details::ValueOrRef<ItemUseEvent>,                       // 2
                 Details::ValueOrRef<ItemUsedOnEvent>,                    // 3
                 Details::ValueOrRef<ItemStartUseOnEvent>,                // 4
                 Details::ValueOrRef<ItemStopUseOnEvent>,                 // 5
                 Details::ValueOrRef<ItemStartUseEvent>,                  // 6
                 Details::ValueOrRef<ItemCompleteUseEvent>,               // 7
                 Details::ValueOrRef<ItemReleaseUseEvent>,                // 8
                 Details::ValueOrRef<ItemStopUseEvent>,                   // 9
                 Details::ValueOrRef<ItemEventPlaceHolder<328> const>>
        event;
};
