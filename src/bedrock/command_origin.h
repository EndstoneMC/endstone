//
// Created by Vincent on 28/08/2023.
//

#ifndef ENDSTONE_COMMAND_ORIGIN_H
#define ENDSTONE_COMMAND_ORIGIN_H

#include "bedrock_common.h"

class CommandOrigin {

public:
    enum class Type : uint8_t {
        Player = 0,
        Block = 1,
        MinecartBlock = 2,
        AutomationPlayer = 5,
        ClientAutomation = 6,
        Server = 7,
        Actor = 8,
        Virtual = 9,
        ActorServer = 11,
        Precompiled = 12,
        GameDirectoryEntityServerCommand = 13,
        Script = 14,
        ExecuteContext = 15,
    };

    Type getOriginType()
    {
        using func_t = Type (*)(CommandOrigin *);
        auto v_table = *reinterpret_cast<char **>(this);
        auto func = *reinterpret_cast<func_t *>(v_table + 23 * 8);
        return func(this);
    }
};

#endif // ENDSTONE_COMMAND_ORIGIN_H
