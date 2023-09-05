//
// Created by Vincent on 02/08/2023.
//

#ifndef ENDSTONE_BEDROCK_LOG_H
#define ENDSTONE_BEDROCK_LOG_H

#include "bedrock_common.h"

#include <cstdarg>
#include <cstdio>

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
}; // namespace BedrockLog

#endif // ENDSTONE_BEDROCK_LOG_H
