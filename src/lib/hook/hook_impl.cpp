//
// Created by Vincent on 02/08/2023.
//

#include "bedrock/bedrock_log.h"
#include "bedrock/command_registry.h"
#include "bedrock/dedicated_server.h"
#include "bedrock/minecraft_commands.h"
#include "bedrock/server_instance.h"
#include "endstone/command/bedrock_command.h"
#include "endstone/endstone.h"
#include "endstone/endstone_server.h"
#include "endstone/logger_factory.h"
#include "hook_manager.h"

void HookManager::registerHooks()
{
    HOOK_FUNCTION(BedrockLog::log_va)
    HOOK_FUNCTION(DedicatedServer::runDedicatedServerLoop)
    HOOK_FUNCTION(DedicatedServer::initializeLogging)
    HOOK_FUNCTION(ServerInstance::startServerThread)
    HOOK_FUNCTION(ServerInstanceEventCoordinator::sendServerThreadStarted)
    HOOK_FUNCTION(ServerInstanceEventCoordinator::sendServerThreadStopped)
    HOOK_FUNCTION(ServerInstanceEventCoordinator::sendServerUpdateEnd)
    HOOK_FUNCTION(MinecraftCommands::executeCommand)
    HOOK_FUNCTION(CommandRegistry::registerAlias)
    HOOK_FUNCTION(CommandRegistry::registerCommand)
}

void ServerInstance::startServerThread()
{
    Endstone::getServer().loadPlugins();
    CALL_ORIGINAL(ServerInstance::startServerThread)
}

void ServerInstanceEventCoordinator::sendServerThreadStarted(ServerInstance *instance)
{
    // Server loop starts
    Endstone::getServer().enablePlugins();
    CALL_ORIGINAL(ServerInstanceEventCoordinator::sendServerThreadStarted, instance)
}

void ServerInstanceEventCoordinator::sendServerThreadStopped(ServerInstance *instance)
{
    // Server loop stops
    Endstone::getServer().disablePlugins();
    CALL_ORIGINAL(ServerInstanceEventCoordinator::sendServerThreadStopped, instance)
}

void ServerInstanceEventCoordinator::sendServerUpdateEnd(ServerInstance *instance)
{
    // Server ticks
    CALL_ORIGINAL(ServerInstanceEventCoordinator::sendServerUpdateEnd, instance)
}

int DedicatedServer::runDedicatedServerLoop(void *file_path_manager, //
                                            void *properties, void *level_settings, void *allow_list, void *permissions)
{
    Endstone::setServer(std::make_unique<EndstoneServer>());
    return CALL_ORIGINAL(DedicatedServer::runDedicatedServerLoop, file_path_manager, properties, level_settings,
                         allow_list, permissions);
}

void DedicatedServer::initializeLogging()
{
    std::vector<std::string> &filters = *static_cast<std::vector<std::string> *>(lookupSymbol("gPriorityFilters"));
    if (std::find(filters.begin(), filters.end(), "VERBOSE") == filters.end()) {
        filters.emplace_back("VERBOSE");
    }

    return CALL_ORIGINAL(DedicatedServer::initializeLogging);
}

void BedrockLog::log_va(BedrockLog::LogCategory category, std::bitset<3> flags, BedrockLog::LogRule rule,
                        LogAreaID area, LogLevel level, const char *function, int line, const char *format,
                        va_list args)
{
    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    CALL_ORIGINAL_STATIC(BedrockLog::log_va, category, flags, rule, area, level, function, line, format, args);
    printf("\x1b[3;32m"); // set to green and italic
}

MinecraftCommands::Result *MinecraftCommands::executeCommand(MinecraftCommands::Result *result,
                                                             CommandContext *command_ctx, bool flag)
{
    constexpr MinecraftCommands::Result success{true, 0, 0};
    constexpr MinecraftCommands::Result not_found{false, 0, 1};

    auto command_line = command_ctx->command_line;
    auto command_name =
        command_line.substr(command_line[0] == '/' ? 1 : 0, command_line.find(' ') - (command_line[0] == '/' ? 1 : 0));

    auto &server = dynamic_cast<EndstoneServer &>(Endstone::getServer());
    auto command = server.getCommandMap().getCommand(command_name);

    if (std::dynamic_pointer_cast<BedrockCommandPlaceHolder>(command)) {
        CALL_ORIGINAL(MinecraftCommands::executeCommand, result, command_ctx, flag)
        return result;
    }
    else {
        // TODO: check origin type and pass the right command sender
        if (server.dispatchCommand(server.getConsoleSender(), command_ctx->command_line)) {
            *result = success;
        }
        else {
            *result = not_found;
        }
    }

    return result;
}

void CommandRegistry::registerAlias(std::string label, std::string alias)
{
    // TODO:
    return CALL_ORIGINAL(CommandRegistry::registerAlias, std::move(label), std::move(alias));
}

void CommandRegistry::registerCommand(const std::string *name, const char *description,
                                      CommandPermissionLevel permission_level, CommandFlag flag1, CommandFlag flag2)
{
    auto server = dynamic_cast<EndstoneServer *>(&Endstone::getServer());
    // put a placeholder so that no plugin can override vanilla commands
    server->getCommandMap().registerOne("minecraft", std::make_shared<BedrockCommandPlaceHolder>(*name));
    return CALL_ORIGINAL(CommandRegistry::registerCommand, name, description, permission_level, flag1, flag2);
}
