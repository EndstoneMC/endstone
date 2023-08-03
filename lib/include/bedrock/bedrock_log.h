//
// Created by Vincent on 02/08/2023.
//

#ifndef ENDSTONE_BEDROCK_LOG_H
#define ENDSTONE_BEDROCK_LOG_H

#include "endstone.h"

#include <cstdarg>
#include <cstdio>

class BedrockLog
{
  public:
    enum class LogAreaID
    {
        ALL,
        PLATFORM_,
        ENTITY,
        DATABASE,
        GUI,
        SYSTEM,
        NETWORK,
        RENDER,
        MEMORY,
        ANIMATION,
        INPUT,
        LEVEL,
        SERVER,
        DLC,
        PHYSICS,
        FILE,
        STORAGE,
        REALMS,
        REALMS_API,
        XBOX_LIVE,
        USER_MANAGER,
        XS_API,
        PERF,
        TELEMETRY,
        BLOCKS,
        RAKNET,
        GAMEFACE,
        SOUND,
        INTERACTIVE,
        SCRIPTING,
        PLAY_FAB,
        AUTOMATION,
        PERSONA,
        TEXTURE,
        ASSET_PACKAGES,
        ITEMS,
        SERVICES,
        VOLUMES,
        LOOT_TABLE,
        SIDEBAR,
        LOCALIZATION,
        MOVEMENT,
        LIVE_EVENTS,
        EDITOR,
        LEVEL_TRANSITION,
        UNKNOWN,
        STORE,
        NUM_LOG_AREA_IDS
    };

    enum class LogCategory
    {
        ALL
    };

    enum class LogRule
    {
        DEFAULT
    };

    enum class LogLevel
    {
        LOG_ALL = -1,
        LOG_VERBOSE = 1,
        LOG_INFO = 2,
        LOG_WARN = 4,
        LOG_ERROR = 8,
    };

    static void log(LogCategory category,
                    std::bitset<3> flags, // unknown
                    LogRule rule,         // unknown
                    LogAreaID area,
                    LogLevel level,
                    char const *function,
                    int line,
                    char const *format,
                    ...);

    static void log_va(LogCategory category,
                       std::bitset<3> flags, // unknown - always 1?
                       LogRule rule,         // 0?
                       LogAreaID area,
                       LogLevel level,
                       char const *function,
                       int line,
                       char const *format,
                       va_list args);
};

#endif // ENDSTONE_BEDROCK_LOG_H
