//
// Created by Vincent on 25/08/2023.
//

#ifndef ENDSTONE_COMMAND_CONTEXT_H
#define ENDSTONE_COMMAND_CONTEXT_H

#include "bedrock_common.h"
#include "command_origin.h"
struct CommandContext {
    std::string command_line;              // +0
    std::unique_ptr<CommandOrigin> sender; // +32
    int command_version;                   // +40
};

static_assert(sizeof(CommandContext) == 48);

#endif // ENDSTONE_COMMAND_CONTEXT_H
