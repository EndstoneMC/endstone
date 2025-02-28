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

#include <string>

namespace Common {
std::string getGameVersionString();
}  // namespace Common

namespace SharedConstants {
static constexpr int TicksPerSecond = 20;
constexpr float SecondsPerTick = 1.0 / TicksPerSecond;
constexpr float MilliSecondsPerTick = SecondsPerTick * 1000;
constexpr int TicksPerMinute = TicksPerSecond * 60;
constexpr int TotalTrialTicks = TicksPerMinute * 90;
constexpr uint8_t NetworkMaxSubClients = 4;
constexpr int NetworkDefaultMaxConnections = 200;  // Endstone: 40 -> 200
}  // namespace SharedConstants
