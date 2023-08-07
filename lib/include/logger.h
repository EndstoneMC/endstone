//
// Created by Vincent on 02/08/2023.
//

#ifndef ENDSTONE_LOGGER_H
#define ENDSTONE_LOGGER_H

#include "bedrock/bedrock_log.h"

/**
 * @brief Macro to log messages in EndStone.
 *
 * Logs a message using the BedrockLog class.
 * The area and level are specified by the caller, as well as the message format and additional arguments.
 *
 * @param area The area of the log (e.g., NETWORK, GAME).
 * @param level The level of the log (e.g., VERBOSE, INFO).
 * @param format The message format string.
 * @param ... The additional arguments for the format string.
 */
#define ENDSTONE_LOG(area, level, format, ...)                                                                         \
    BedrockLog::log(BedrockLog::LogCategory::LOG_ALL,                                                                  \
                    1,                                                                                                 \
                    BedrockLog::LogRule::LOG_DEFAULT,                                                                  \
                    BedrockLog::LogAreaID::LOG_##area,                                                                 \
                    BedrockLog::LogLevel::LOG_##level,                                                                 \
                    __FUNCTION__,                                                                                      \
                    __LINE__,                                                                                          \
                    "[EndStone] " format,                                                                              \
                    ##__VA_ARGS__);

/**
 * @brief Log a verbose message.
 * @param area The area of the log.
 * @param format The message format string.
 * @param ... The additional arguments for the format string.
 */
#define ENDSTONE_VERBOSE(area, format, ...) ENDSTONE_LOG(area, VERBOSE, format, ##__VA_ARGS__)

/**
 * @brief Log an informational message.
 * @param area The area of the log.
 * @param format The message format string.
 * @param ... The additional arguments for the format string.
 */
#define ENDSTONE_INFO(area, format, ...) ENDSTONE_LOG(area, INFO, format, ##__VA_ARGS__)

/**
 * @brief Log a warning message.
 * @param area The area of the log.
 * @param format The message format string.
 * @param ... The additional arguments for the format string.
 */
#define ENDSTONE_WARN(area, format, ...) ENDSTONE_LOG(area, WARN, format, ##__VA_ARGS__)

/**
 * @brief Log an error message.
 * @param area The area of the log.
 * @param format The message format string.
 * @param ... The additional arguments for the format string.
 */
#define ENDSTONE_ERROR(area, format, ...) ENDSTONE_LOG(area, ERROR, format, ##__VA_ARGS__)

#endif // ENDSTONE_LOGGER_H
