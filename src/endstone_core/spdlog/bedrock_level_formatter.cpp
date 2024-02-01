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

#include "endstone_core/spdlog/bedrock_level_formatter.h"

#include <spdlog/details/fmt_helper.h>

void BedrockLevelFormatter::format(const spdlog::details::log_msg &msg, const tm &, spdlog::memory_buf_t &dest)
{
    static const std::unordered_map<spdlog::level::level_enum, std::string_view> level_names = {
        {spdlog::level::trace, "TRACE"},  {spdlog::level::debug, "DEBUG"}, {spdlog::level::info, "INFO"},
        {spdlog::level::warn, "WARNING"}, {spdlog::level::err, "ERROR"},   {spdlog::level::critical, "CRITICAL"},
        {spdlog::level::off, "OFF"},
    };

    const auto level_name = level_names.find(msg.level)->second;
    spdlog::details::fmt_helper::append_string_view(level_name, dest);
}

std::unique_ptr<spdlog::custom_flag_formatter> BedrockLevelFormatter::clone() const
{
    return spdlog::details::make_unique<BedrockLevelFormatter>();
}
