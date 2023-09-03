//
// Created by Vincent on 02/08/2023.
//

#include "bedrock/bedrock_log.h"
#include "bedrock/command_registry.h"
#include "bedrock/dedicated_server.h"
#include "bedrock/minecraft_commands.h"
#include "bedrock/server_instance.h"
#include "endstone/chat_color.h"
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
        {ChatColors::toString(ChatColor::Black), "\x1b[30m"},
        {ChatColors::toString(ChatColor::DarkBlue), "\x1b[34m"},
        {ChatColors::toString(ChatColor::DarkGreen), "\x1b[32m"},
        {ChatColors::toString(ChatColor::DarkAqua), "\x1b[36m"},
        {ChatColors::toString(ChatColor::DarkRed), "\x1b[31m"},
        {ChatColors::toString(ChatColor::DarkPurple), "\x1b[35m"},
        {ChatColors::toString(ChatColor::Gold), "\x1b[33m"},
        {ChatColors::toString(ChatColor::Gray), "\x1b[37m"},
        {ChatColors::toString(ChatColor::DarkGray), "\x1b[90m"},
        {ChatColors::toString(ChatColor::Blue), "\x1b[94m"},
        {ChatColors::toString(ChatColor::Green), "\x1b[92m"},
        {ChatColors::toString(ChatColor::Aqua), "\x1b[96m"},
        {ChatColors::toString(ChatColor::Red), "\x1b[91m"},
        {ChatColors::toString(ChatColor::LightPurple), "\x1b[95m"},
        {ChatColors::toString(ChatColor::Yellow), "\x1b[93m"},
        {ChatColors::toString(ChatColor::White), "\x1b[97m"},
        {ChatColors::toString(ChatColor::MinecoinGold), "\x1b[38;2;221;214;5m"},
        {ChatColors::toString(ChatColor::MaterialQuartz), "\x1b[38;2;227;212;209m"},
        {ChatColors::toString(ChatColor::MaterialIron), "\x1b[38;2;206;202;202m"},
        {ChatColors::toString(ChatColor::MaterialNetherite), "\x1b[38;2;68;58;59m"},
        {ChatColors::toString(ChatColor::Magic), "\x1b[8m"},
        {ChatColors::toString(ChatColor::Bold), "\x1b[1m"},
        {ChatColors::toString(ChatColor::MaterialRedstone), "\x1b[38;2;151;22;7m"},
        {ChatColors::toString(ChatColor::MaterialCopper), "\x1b[38;2;180;104;77m"},
        {ChatColors::toString(ChatColor::Italic), "\x1b[3m"},
        {ChatColors::toString(ChatColor::MaterialGold), "\x1b[38;2;222;177;45m"},
        {ChatColors::toString(ChatColor::MaterialEmerald), "\x1b[38;2;222;177;45m"},
        {ChatColors::toString(ChatColor::Reset), "\x1b[0m"},
        {ChatColors::toString(ChatColor::MaterialDiamond), "\x1b[38;2;44;186;168m"},
        {ChatColors::toString(ChatColor::MaterialLapis), "\x1b[38;2;33;73;123m"},
        {ChatColors::toString(ChatColor::MaterialAmethyst), "\x1b[38;2;154;92;198m"},
    };

    std::string_view subject = format;
    std::string output;
    output.reserve(strlen(format) * 2);
    std::ptrdiff_t last_pos = 0;
    std::match_results<std::string_view::const_iterator> match;

    while (std::regex_search(subject.begin() + last_pos, subject.end(), match, ChatColors::PATTERN)) {
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
    printf("\x1b[0;3;32m"); // set to green and italic
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
        // replace the fallback prefix in a command (e.g. /minecraft:) with a forward slash (i.e. /)
        command_ctx->command_line = std::regex_replace(command_ctx->command_line, std::regex("^/(\\w+):"), "/");
        CALL_ORIGINAL(MinecraftCommands::executeCommand, result, command_ctx, flag)
        return result;
    }
    else {
        // TODO: check origin type and pass the right command sender (e.g. PlayerCommandSender)
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
    auto command = commands[name];
    auto aliases = command->getAliases();
    aliases.push_back(alias);
    command->setAliases(aliases);
    return CALL_ORIGINAL(CommandRegistry::registerAlias, std::move(name), std::move(alias));
}

void CommandRegistry::registerCommand(const std::string *name, const char *description,
                                      CommandPermissionLevel permission_level, CommandFlag flag1, CommandFlag flag2)
{
    commands[*name] = std::make_shared<BedrockCommandPlaceHolder>(*name);
    return CALL_ORIGINAL(CommandRegistry::registerCommand, name, description, permission_level, flag1, flag2);
}
