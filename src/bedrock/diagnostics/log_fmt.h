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

#include <format>
#include <string_view>

#include "bedrock/diagnostics/log_area.h"
#include "bedrock/diagnostics/log_level.h"

template <>
struct std::formatter<LogAreaID> : std::formatter<std::string_view> {
    std::format_context::iterator format(LogAreaID val, std::format_context &ctx) const;
};

template <>
struct std::formatter<Bedrock::LogLevel> : std::formatter<std::string_view> {
    std::format_context::iterator format(Bedrock::LogLevel val, std::format_context &ctx) const;
};
