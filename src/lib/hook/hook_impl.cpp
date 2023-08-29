//
// Created by Vincent on 02/08/2023.
//

#include "bedrock/bedrock_log.h"
#include "bedrock/command_registry.h"
#include "bedrock/dedicated_server.h"
#include "bedrock/minecraft_commands.h"
#include "bedrock/server_instance.h"
#include "endstone/chat_colors.h"
#include "endstone/command/bedrock_command.h"
#include "endstone/common.h"
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
    static std::unordered_map<std::string, std::string> replacements = {
        {ChatColors::BLACK.toString(), "\x1b[0;30m"},                          // 0
        {ChatColors::DARK_BLUE.toString(), "\x1b[0;34m"},                      // 1
        {ChatColors::DARK_GREEN.toString(), "\x1b[0;32m"},                     // 2
        {ChatColors::DARK_AQUA.toString(), "\x1b[0;36m"},                      // 3
        {ChatColors::DARK_RED.toString(), "\x1b[0;31m"},                       // 4
        {ChatColors::DARK_PURPLE.toString(), "\x1b[0;35m"},                    // 5
        {ChatColors::GOLD.toString(), "\x1b[0;33m"},                           // 6
        {ChatColors::GRAY.toString(), "\x1b[0;37m"},                           // 7
        {ChatColors::DARK_GRAY.toString(), "\x1b[0;90m"},                      // 8
        {ChatColors::BLUE.toString(), "\x1b[0;94m"},                           // 9
        {ChatColors::GREEN.toString(), "\x1b[0;92m"},                          // a
        {ChatColors::AQUA.toString(), "\x1b[0;96m"},                           // b
        {ChatColors::RED.toString(), "\x1b[0;91m"},                            // c
        {ChatColors::LIGHT_PURPLE.toString(), "\x1b[0;95m"},                   // d
        {ChatColors::YELLOW.toString(), "\x1b[0;93m"},                         // e
        {ChatColors::WHITE.toString(), "\x1b[0;97m"},                          // f
        {ChatColors::MINECOIN_GOLD.toString(), "\x1b[0;38;2;221;214;5m"},      // g
        {ChatColors::MATERIAL_QUARTZ.toString(), "\x1b[0;38;2;227;212;209m"},  // h
        {ChatColors::MATERIAL_IRON.toString(), "\x1b[0;38;2;206;202;202m"},    // i
        {ChatColors::MATERIAL_NETHERITE.toString(), "\x1b[0;38;2;68;58;59m"},  // j
        {ChatColors::MAGIC.toString(), "\x1b[8m"},                             // k
        {ChatColors::BOLD.toString(), "\x1b[1m"},                              // l
        {ChatColors::MATERIAL_REDSTONE.toString(), "\x1b[0;38;2;151;22;7m"},   // m
        {ChatColors::MATERIAL_COPPER.toString(), "\x1b[0;38;2;180;104;77m"},   // n
        {ChatColors::ITALIC.toString(), "\x1b[3m"},                            // o
        {ChatColors::MATERIAL_GOLD.toString(), "\x1b[0;38;2;222;177;45m"},     // p
        {ChatColors::MATERIAL_EMERALD.toString(), "\x1b[0;38;2;222;177;45m"},  // q
        {ChatColors::RESET.toString(), "\x1b[0m"},                             // r
        {ChatColors::MATERIAL_DIAMOND.toString(), "\x1b[0;38;2;44;186;168m"},  // s
        {ChatColors::MATERIAL_LAPIS.toString(), "\x1b[0;38;2;33;73;123m"},     // t
        {ChatColors::MATERIAL_AMETHYST.toString(), "\x1b[0;38;2;154;92;198m"}, // u
    };

    std::string_view subject = format;
    std::string output;
    output.reserve(strlen(format) * 2);
    std::ptrdiff_t last_pos = 0;
    std::match_results<std::string_view::const_iterator> match;

    while (std::regex_search(subject.begin() + last_pos, subject.end(), match, ChatColor::COLOR_PATTERN)) {
        output.append(subject.substr(last_pos, match.position()));

        auto it = replacements.find(match.str());
        if (it != replacements.end()) {
            output.append(it->second);
        }
        else {
            output.append(match.str());
        }

        last_pos += (match.position() + match.length());
    }
    output.append(subject.substr(last_pos));

    static std::mutex mtx;
    std::lock_guard<std::mutex> lock(mtx);
    CALL_ORIGINAL_STATIC(BedrockLog::log_va, category, flags, rule, area, level, function, line, output.c_str(), args);
    printf("\x1b[3;32m"); // set to green and italic
}

MinecraftCommands::Result *MinecraftCommands::executeCommand(MinecraftCommands::Result *result,
                                                             CommandContext *command_ctx, bool flag)
{
    constexpr MinecraftCommands::Result success{true, 0, 0};
    constexpr MinecraftCommands::Result not_found{false, 0, 1};

    auto command_name = CommandMap::getCommandName(command_ctx->command_line);
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

void CommandRegistry::registerAlias(std::string name, std::string alias)
{
    auto &server = dynamic_cast<EndstoneServer &>(Endstone::getServer());
    auto command = std::dynamic_pointer_cast<BedrockCommandPlaceHolder>(server.getCommandMap().getCommand(name));
    server.getCommandMap().registerCommand(alias, command, true, "minecraft");

    return CALL_ORIGINAL(CommandRegistry::registerAlias, std::move(name), std::move(alias));
}

void CommandRegistry::registerCommand(const std::string *name, const char *description,
                                      CommandPermissionLevel permission_level, CommandFlag flag1, CommandFlag flag2)
{
    auto server = dynamic_cast<EndstoneServer *>(&Endstone::getServer());
    // put a placeholder so that no plugin can override vanilla commands
    server->getCommandMap().registerCommand("minecraft", std::make_shared<BedrockCommandPlaceHolder>(*name));
    return CALL_ORIGINAL(CommandRegistry::registerCommand, name, description, permission_level, flag1, flag2);
}
