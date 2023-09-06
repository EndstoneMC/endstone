//
// Created by Vincent on 06/09/2023.
//

#include "bedrock/command_registry.h"

#include "bedrock/i18n.h"
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
    bedrock_commands[name] = std::make_shared<BedrockCommandPlaceHolder>(
        name, I18n::get(description), std::vector<std::string>{"/" + name}, std::vector<std::string>{});
    ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::registerCommand, this, name, description, level, flag1, flag2);
}

void CommandRegistry::registerOverloadInternal(CommandRegistry::Signature &signature,
                                               CommandRegistry::Overload &overload)
{
    printf("/%s ", signature.label.c_str());
    for (const auto &parameter : overload.parameters) {
        printf("%s ", describe(parameter).c_str());
    }
    printf("\n");
    ENDSTONE_HOOK_CALL_ORIGINAL(&CommandRegistry::registerOverloadInternal, this, signature, overload);
}

std::string CommandRegistry::describe(const CommandParameterData &parameter) const
{
    std::string result;
    result = *ENDSTONE_HOOK_CALL_ORIGINAL_RVO(&CommandRegistry::describe, this, &result, parameter);
    return result;
}
