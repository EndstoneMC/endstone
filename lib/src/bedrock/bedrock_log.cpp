//
// Created by Vincent on 02/08/2023.
//

#include "bedrock/bedrock_log.h"

void BedrockLog::log(LogCategory category,
                     std::bitset<3> flags,
                     LogRule rule,
                     LogAreaID area,
                     LogLevel level,
                     const char *function,
                     int line,
                     const char *format,
                     ...)
{
    va_list args;
    va_start(args, format);
    log_va(category, flags, rule, area, level, function, line, format, args);
    va_end(args);
}
