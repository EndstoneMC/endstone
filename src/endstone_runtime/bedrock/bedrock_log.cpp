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

#include "bedrock/bedrock_log.h"

#include <cctype>
#include <cstdio>
#include <unordered_map>

#include <magic_enum/magic_enum.hpp>

#include "endstone/detail/logger_factory.h"
#include "endstone/logger.h"

void BedrockLog::log_va(BedrockLog::LogCategory /*category*/, std::bitset<3> /*flags*/, BedrockLog::LogRule /*rule*/,
                        LogAreaID area, LogLevel level, const char * /*function*/, int /*line*/, const char *format,
                        va_list args)
{
    auto name = magic_enum::enum_name(area);
    auto &logger = endstone::detail::LoggerFactory::getLogger(std::string(name));

    static const std::unordered_map<LogLevel, endstone::Logger::Level> log_levels = {
        {1, endstone::Logger::Level::Debug},
        {2, endstone::Logger::Level::Info},
        {4, endstone::Logger::Level::Warning},
        {8, endstone::Logger::Level::Error},
    };

    endstone::Logger::Level log_level = endstone::Logger::Level::Critical;
    auto iter = log_levels.find(level);
    if (iter != log_levels.end()) {
        log_level = iter->second;
    }

    std::va_list args_copy;
    va_copy(args_copy, args);
    std::vector<char> buf(1 + std::vsnprintf(nullptr, 0, format, args));
    va_end(args);
    size_t len = std::vsnprintf(buf.data(), buf.size(), format, args_copy);
    va_end(args_copy);

    std::string message(buf.data(), len);
    message.erase(message.find_last_not_of(" \t\n\r\f\v") + 1);

    std::string_view sv(message);
    size_t start = 0;
    size_t end;

    while ((end = sv.find('\n', start)) != std::string::npos) {
        logger.log(log_level, sv.substr(start, end - start));
        start = end + 1;
    }

    if (start < sv.length()) {
        logger.log(log_level, sv.substr(start));
    }
}
