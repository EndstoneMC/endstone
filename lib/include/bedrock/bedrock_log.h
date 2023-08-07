//
// Created by Vincent on 02/08/2023.
//

#ifndef ENDSTONE_BEDROCK_LOG_H
#define ENDSTONE_BEDROCK_LOG_H

#include "endstone.h"

#include <cstdarg>
#include <cstdio>

/**
 * @class BedrockLog
 * @brief Provides logging functionalities with different levels and categories.
 */
class BedrockLog
{
  public:
    /**
     * @enum LogAreaID
     * @brief Identifies different logging areas within the system.
     */
    enum class LogAreaID
    {
        LOG_ALL,
        LOG_PLATFORM,
        LOG_ENTITY,
        LOG_DATABASE,
        LOG_GUI,
        LOG_SYSTEM,
        LOG_NETWORK,
        LOG_RENDER,
        LOG_MEMORY,
        LOG_ANIMATION,
        LOG_INPUT,
        LOG_LEVEL,
        LOG_SERVER,
        LOG_DLC,
        LOG_PHYSICS,
        LOG_FILE,
        LOG_STORAGE,
        LOG_REALMS,
        LOG_REALMS_API,
        LOG_XBOX_LIVE,
        LOG_USER_MANAGER,
        LOG_XS_API,
        LOG_PERF,
        LOG_TELEMETRY,
        LOG_BLOCKS,
        LOG_RAKNET,
        LOG_GAMEFACE,
        LOG_SOUND,
        LOG_INTERACTIVE,
        LOG_SCRIPTING,
        LOG_PLAY_FAB,
        LOG_AUTOMATION,
        LOG_PERSONA,
        LOG_TEXTURE,
        LOG_ASSET_PACKAGES,
        LOG_ITEMS,
        LOG_SERVICES,
        LOG_VOLUMES,
        LOG_LOOT_TABLE,
        LOG_SIDEBAR,
        LOG_LOCALIZATION,
        LOG_MOVEMENT,
        LOG_LIVE_EVENTS,
        LOG_EDITOR,
        LOG_LEVEL_TRANSITION,
        LOG_UNKNOWN,
        LOG_STORE
    };

    /**
     * @enum LogCategory
     * @brief Identifies logging categories.
     */
    enum class LogCategory
    {
        LOG_ALL
    };

    /**
     * @enum LogRule
     * @brief Represents specific rules for logging.
     */
    enum class LogRule
    {
        LOG_DEFAULT
    };

    /**
     * @enum LogLevel
     * @brief Specifies the log level.
     */
    enum class LogLevel
    {
        LOG_ALL = -1,
        LOG_VERBOSE = 1,
        LOG_INFO = 2,
        LOG_WARN = 4,
        LOG_ERROR = 8,
    };

    /**
     * @brief Logs a message with the given parameters.
     * @param category Logging category.
     * @param flags Bitset of flags.
     * @param rule Logging rule.
     * @param area Logging area identifier.
     * @param level Logging level.
     * @param function Function name.
     * @param line Line number.
     * @param format Format string for the log message.
     * @param ... Additional arguments for the format string.
     */
    static void log(LogCategory category,
                    std::bitset<3> flags,
                    LogRule rule,
                    LogAreaID area,
                    LogLevel level,
                    char const *function,
                    int line,
                    char const *format,
                    ...);

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
    static void log_va(LogCategory category,
                       std::bitset<3> flags,
                       LogRule rule,
                       LogAreaID area,
                       LogLevel level,
                       char const *function,
                       int line,
                       char const *format,
                       va_list args);
};

#endif // ENDSTONE_BEDROCK_LOG_H
