//
// Created by Vincent on 28/08/2023.
//

#ifndef ENDSTONE_COMMAND_REGISTRY_H
#define ENDSTONE_COMMAND_REGISTRY_H

#include "bedrock_common.h"
#include "endstone/command/command.h"

struct CommandFlag {
    uint16_t value;
};

class CommandRegistry {
public:
    BEDROCK_API void registerAlias(std::string name, std::string alias);
    BEDROCK_API void registerCommand(const std::string &name, char const *description,
                                     enum class CommandPermissionLevel level, CommandFlag flag1, CommandFlag flag2);

    inline static std::map<std::string, std::shared_ptr<Command>> bedrock_commands{};
};

#endif // ENDSTONE_COMMAND_REGISTRY_H
