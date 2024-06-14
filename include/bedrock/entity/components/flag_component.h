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

template <typename ComponentFlag>
class FlagComponent {
public:
    ComponentFlag flag;
};

struct PlayerComponentFlag {};
struct OnGroundFlagComponent {};
struct WasOnGroundFlag {};
struct InWaterFlag {};
struct InLavaFlag {};
struct MobIsJumpingFlag {};

#if defined __clang__ || defined __GNUC__
static_assert(entt::type_hash<FlagComponent<PlayerComponentFlag>>::value() == 0x356FFFCA);
#elif defined _MSC_VER
static_assert(entt::type_hash<FlagComponent<PlayerComponentFlag>>::value() == 0x600FBE1D);
static_assert(entt::type_hash<OnGroundFlagComponent>::value() == 0xC3E299A3);
#endif
