//
// Created by Vincent on 28/08/2023.
//

#ifndef ENDSTONE_COMMAND_REGISTRY_H
#define ENDSTONE_COMMAND_REGISTRY_H

#include "bedrock_types.h"
#include "endstone/command/command.h"
#include "endstone/common.h"

class CommandRegistry {

public:
    void registerAlias(std::string name, std::string alias);
    void registerCommand(const std::string *name, char const *description, CommandPermissionLevel permission_level,
                         CommandFlag flag1, CommandFlag flag2);

    inline static std::map<std::string, std::shared_ptr<Command>> commands{};
};

#endif // ENDSTONE_COMMAND_REGISTRY_H
