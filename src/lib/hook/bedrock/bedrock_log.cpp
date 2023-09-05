//
// Created by Vincent on 05/09/2023.
//

#include "bedrock/bedrock_log.h"

void BedrockLog::log_va(BedrockLog::LogCategory category, std::bitset<3> flags, BedrockLog::LogRule rule,
                        LogAreaID area, LogLevel level, char const *function, int line, char const *format,
                        va_list args)
{
}
