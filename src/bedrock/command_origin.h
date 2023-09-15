//
// Created by Vincent on 28/08/2023.
//

#pragma once

#include "bedrock_common.h"

enum class CommandOriginType : char {
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

class CommandOrigin {

public:
    CommandOriginType getOriginType() const
    {
        return BEDROCK_VIRTUAL_CALL(23, &CommandOrigin::getOriginType, this);
    }

    std::string getName() const
    {
        std::string name;
        name = *BEDROCK_VIRTUAL_CALL_RVO(2, &CommandOrigin::getName, this, &name);
        return name;
    }
};

