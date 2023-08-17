//
// Created by Vincent on 02/08/2023.
//

#include "bedrock/bedrock_log.h"
#include "bedrock/dedicated_server.h"
#include "bedrock/server_instance.h"
#include "endstone/server.h"
#include "hook/hook_manager.h"

void HookManager::registerHooks()
{
    HOOK_FUNCTION(BedrockLog::log_va)
    //    HOOK_FUNCTION(DedicatedServer::runDedicatedServerLoop)
    HOOK_FUNCTION(DedicatedServer::initializeLogging)
    HOOK_FUNCTION(ServerInstance::startServerThread)
    //    HOOK_FUNCTION(ServerInstanceEventCoordinator::sendServerThreadStarted)
    HOOK_FUNCTION(ServerInstanceEventCoordinator::sendServerThreadStopped)
    HOOK_FUNCTION(ServerInstanceEventCoordinator::sendServerUpdateEnd)
}

void ServerInstance::startServerThread()
{
    Server::getInstance().start();
    Server::getInstance().loadPlugins();
    CALL_ORIGINAL(ServerInstance::startServerThread)
    Server::getInstance().enablePlugins();
}

void ServerInstanceEventCoordinator::sendServerThreadStarted(ServerInstance *serverInstance)
{
    // Server loop starts
    // Server::getInstance().enablePlugins();
    CALL_ORIGINAL(ServerInstanceEventCoordinator::sendServerThreadStarted, serverInstance)
}

void ServerInstanceEventCoordinator::sendServerThreadStopped(ServerInstance *serverInstance)
{
    // Server loop stops
    Server::getInstance().disablePlugins();
    CALL_ORIGINAL(ServerInstanceEventCoordinator::sendServerThreadStopped, serverInstance)
}

void ServerInstanceEventCoordinator::sendServerUpdateEnd(ServerInstance *serverInstance)
{
    // Server ticks
    CALL_ORIGINAL(ServerInstanceEventCoordinator::sendServerUpdateEnd, serverInstance)
}

int DedicatedServer::runDedicatedServerLoop(void *file_path_manager, //
                                            void *properties,
                                            void *level_settings,
                                            void *allow_list,
                                            void *permissions)
{
    // Server::getInstance().start();
    return CALL_ORIGINAL(DedicatedServer::runDedicatedServerLoop,
                         file_path_manager,
                         properties,
                         level_settings,
                         allow_list,
                         permissions);
}

void DedicatedServer::initializeLogging()
{
    std::vector<std::string> &filters = *static_cast<std::vector<std::string> *>(lookupSymbol("gPriorityFilters"));
    if (std::find(filters.begin(), filters.end(), "VERBOSE") == filters.end())
    {
        filters.emplace_back("VERBOSE");
    }

    return CALL_ORIGINAL(DedicatedServer::initializeLogging);
}

void BedrockLog::log_va(BedrockLog::LogCategory category,
                        std::bitset<3> flags,
                        BedrockLog::LogRule rule,
                        LogAreaID area,
                        LogLevel level,
                        const char *function,
                        int line,
                        const char *format,
                        va_list args)
{
    CALL_ORIGINAL_STATIC(BedrockLog::log_va, category, flags, rule, area, level, function, line, format, args);
}