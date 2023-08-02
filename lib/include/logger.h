//
// Created by Vincent on 02/08/2023.
//

#ifndef ENDSTONE_LOGGER_H
#define ENDSTONE_LOGGER_H

#include "bedrock/bedrock_log.h"

#define ENDSTONE_LOG(area, level, format, ...)                                                                         \
    BedrockLog::log(BedrockLog::LogCategory::ALL,                                                                      \
                    1,                                                                                                 \
                    BedrockLog::LogRule::DEFAULT,                                                                      \
                    BedrockLog::LogAreaID::area,                                                                       \
                    BedrockLog::LogLevel::LOG_##level,                                                                 \
                    __FUNCTION__,                                                                                      \
                    __LINE__,                                                                                          \
                    "[EndStone] " format,                                                                              \
                    ##__VA_ARGS__);

#define ENDSTONE_VERBOSE(area, format, ...) ENDSTONE_LOG(area, VERBOSE, format, ##__VA_ARGS__)

#define ENDSTONE_INFO(area, format, ...) ENDSTONE_LOG(area, INFO, format, ##__VA_ARGS__)

#define ENDSTONE_WARN(area, format, ...) ENDSTONE_LOG(area, WARN, format, ##__VA_ARGS__)

#define ENDSTONE_ERROR(area, format, ...) ENDSTONE_LOG(area, ERROR, format, ##__VA_ARGS__)

#endif // ENDSTONE_LOGGER_H
