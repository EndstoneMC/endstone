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

#include "bedrock/diagnostics/log_fmt.h"

#include <magic_enum/magic_enum.hpp>

std::format_context::iterator std::formatter<LogAreaID>::format(LogAreaID val, std::format_context &ctx) const
{
    return std::formatter<std::string_view>::format(magic_enum::enum_name(val), ctx);
}

std::format_context::iterator std::formatter<Bedrock::LogLevel>::format(Bedrock::LogLevel val,
                                                                        std::format_context &ctx) const
{
    return std::formatter<std::string_view>::format(magic_enum::enum_name(val.getType()), ctx);
}
