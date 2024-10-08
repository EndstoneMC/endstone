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

#include <bitset>
#include <cstdarg>

#include "bedrock/bedrock.h"
#include "bedrock/diagnostics/log_area.h"

namespace BedrockLog {
ENDSTONE_HOOK void log_va(LogCategory category, std::bitset<3> channel_mask, LogRule rule, LogAreaID area,
                          std::uint32_t priority, char const *function, int line, char const *format, va_list args);
}  // namespace BedrockLog
