//
// Created by Vincent on 06/09/2023.
//

#include "bedrock/command_registry.h"

#include "endstone/command/bedrock_command.h"
#include "hook/hook.h"

void CommandRegistry::registerAlias(std::string name, std::string alias)
{
    auto command = bedrock_commands[name];
    auto aliases = command->getAliases();
    aliases.push_back(alias);
    command->setAliases(aliases);
    ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::registerAlias, this, std::move(name), std::move(alias));
}
void CommandRegistry::registerCommand(const std::string &name, const char *description,
                                      enum CommandPermissionLevel level, CommandFlag flag1, CommandFlag flag2)
{
    bedrock_commands[name] = std::make_shared<BedrockCommandPlaceHolder>(name);
    ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::registerCommand, this, name, description, level, flag1, flag2);
}
