// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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
#include <cstdio>

#include "bedrock/internal.h"

using LogLevel = uint32_t;

/**
 * @enum LogAreaID
 * @brief Identifies different logging areas within the system.
 */
enum class LogAreaID {
    All,
    Platform,
    Entity,
    Database,
    GUI,
    System,
    Network,
    Render,
    Memory,
    Animation,
    Input,
    Level,
    Server,
    DLC,
    Physics,
    File,
    Storage,
    Realms,
    RealmsAPI,
    XboxLive,
    UserManager,
    XsAPI,
    Perf,
    Telemetry,
    Blocks,
    RakNet,
    Gameface,
    Sound,
    Interactive,
    Scripting,
    PlayFab,
    Automation,
    Persona,
    Texture,
    AssetPackages,
    Items,
    Services,
    Volumes,
    LootTable,
    Sidebar,
    Localization,
    Movement,
    LiveEvents,
    Editor,
    LevelTransition,
    Unknown,
    Store
};

/**
 * @class BedrockLog
 * @brief Provides logging functionalities with different levels and categories.
 */
namespace BedrockLog {
/**
 * @enum LogCategory
 * @brief Identifies logging categories.
 */
enum class LogCategory {
    All
};

/**
 * @enum LogRule
 * @brief Represents specific rules for logging.
 */
enum class LogRule {
    Default
};

/**
 * @brief Logs a message with a variable argument list.
 * @param category Logging category.
 * @param flags Bitset of flags.
 * @param rule Logging rule.
 * @param area Logging area identifier.
 * @param level Logging level.
 * @param function Function name.
 * @param line Line number.
 * @param format Format string for the log message.
 * @param args Variable argument list.
 */
BEDROCK_API void log_va(BedrockLog::LogCategory category, std::bitset<3> flags, BedrockLog::LogRule rule,
                        LogAreaID area, LogLevel level, char const *function, int line, char const *format,
                        va_list args);
}  // namespace BedrockLog
