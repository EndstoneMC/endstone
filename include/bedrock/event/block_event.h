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

template <typename Return>
struct BlockGameplayEvent;

struct BlockPatternPostEvent {};

template <>
struct BlockGameplayEvent<void> {
    std::variant<Details::ValueOrRef<BlockPatternPostEvent const>,
                 Details::ValueOrRef<ActorEventPlaceHolder<_WIN32_LINUX_(224, 192)> const>>
        event;
};

struct ActorInsideBlockEvent {};
struct ActorStandOnBlockEvent {};
struct BlockPatternPreEvent {};
struct BlockRandomTickEvent {};
struct ChestBlockTryPairEvent {};
struct PistonActionEvent {};
struct LeverActionEvent {};
struct ButtonPushEvent {};
struct PressurePlatePushEvent {};
struct PressurePlatePopEvent {};
struct TargetBlockHitEvent {};
struct TripWireTripEvent {};
struct BlockTryPlaceByPlayerEvent {};

template <>
struct BlockGameplayEvent<CoordinatorResult> {
    std::variant<Details::ValueOrRef<ActorInsideBlockEvent const>,       // 0
                 Details::ValueOrRef<ActorStandOnBlockEvent const>,      // 1
                 Details::ValueOrRef<BlockPatternPreEvent const>,        // 2
                 Details::ValueOrRef<BlockRandomTickEvent const>,        // 3
                 Details::ValueOrRef<ChestBlockTryPairEvent const>,      // 4
                 Details::ValueOrRef<PistonActionEvent const>,           // 5
                 Details::ValueOrRef<LeverActionEvent const>,            // 6
                 Details::ValueOrRef<ButtonPushEvent const>,             // 7
                 Details::ValueOrRef<PressurePlatePushEvent const>,      // 8
                 Details::ValueOrRef<PressurePlatePopEvent const>,       // 9
                 Details::ValueOrRef<TargetBlockHitEvent const>,         // 10
                 Details::ValueOrRef<TripWireTripEvent const>,           // 11
                 Details::ValueOrRef<BlockTryPlaceByPlayerEvent const>,  // 12
#ifdef _WIN32
                 Details::ValueOrRef<ActorEventPlaceHolder<232> const>>
#else
                 Details::ValueOrRef<ActorEventPlaceHolder<192> const>>
#endif
        event;
};
