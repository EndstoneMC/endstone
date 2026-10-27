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

#include "bedrock/core/debug/bedrock_event_logger.h"

#include <ranges>
#include <string_view>

#include <magic_enum/magic_enum.hpp>

#include "bedrock/diagnostics/log_level.h"
#include "endstone/core/logger_factory.h"
#include "endstone/logger.h"

void BedrockLog::LogDetails::_log(LogAreaID area, unsigned int priority, const char *function, int /*line*/,
                                  MessasgeId /*message_id*/, std::string message_text)
{
    if ((area == LogAreaID::Database || area == LogAreaID::System) &&
        priority <= static_cast<unsigned int>(Bedrock::LogLevel::Type::Info)) {
        // Suppress logs from DBStorage and from ContentLog with unknown area (mapped to System)
        return;
    }

    if (area == LogAreaID::Server && priority == static_cast<unsigned int>(Bedrock::LogLevel::Type::Error) &&
        function && std::string_view(function).ends_with("NotUsingNetherNet")) {
        // Suppress the transport type error when running on RakNet
        return;
    }

    static auto &logger = endstone::core::LoggerFactory::getLogger("");
    endstone::Logger::Level log_level;
    switch (priority) {
    case static_cast<unsigned int>(Bedrock::LogLevel::Type::Verbose):
        log_level = endstone::Logger::Level::Debug;
        break;
    case static_cast<unsigned int>(Bedrock::LogLevel::Type::Info):
        log_level = endstone::Logger::Level::Info;
        break;
    case static_cast<unsigned int>(Bedrock::LogLevel::Type::Warning):
        log_level = endstone::Logger::Level::Warning;
        break;
    case static_cast<unsigned int>(Bedrock::LogLevel::Type::Error):
        log_level = endstone::Logger::Level::Error;
        break;
    default:
        log_level = endstone::Logger::Level::Critical;
        break;
    }

    std::string_view message(message_text);
    if (area == LogAreaID::Scripting) {
        // script output comes with a trailing "\n\r"
        message = message.substr(0, message.find_last_not_of("\r\n") + 1);
    }

    auto lines = message | std::ranges::views::split('\n');
    for (const auto &l : lines) {
        logger.log(log_level, std::string_view(l.begin(), l.end()));
    }
}
