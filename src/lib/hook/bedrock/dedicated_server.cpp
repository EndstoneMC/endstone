//
// Created by Vincent on 05/09/2023.
//

#include "bedrock/dedicated_server.h"

#include "endstone/endstone.h"
#include "endstone/endstone_server.h"
#include "hook/hook.h"

void DedicatedServer::initializeLogging()
{
    auto filters = endstone::hook::sym_from_name<std::vector<std::string> *>("gPriorityFilters");
    if (std::find(filters->begin(), filters->end(), "VERBOSE") == filters->end()) {
        filters->emplace_back("VERBOSE");
    }

    ENDSTONE_HOOK_CALL_ORIGINAL(&DedicatedServer::initializeLogging, this);
}

DedicatedServer::StartResult DedicatedServer::runDedicatedServerLoop(Core::FilePathManager &file_path_manager,
                                                                     PropertiesSettings &properties_settings,
                                                                     LevelSettings &level_settings,
                                                                     AllowListFile &allow_list_file,
                                                                     std::unique_ptr<PermissionsFile> &permissions_file)
{
    Endstone::setServer(std::make_unique<EndstoneServer>());
    return ENDSTONE_HOOK_CALL_ORIGINAL(&DedicatedServer::runDedicatedServerLoop, this, file_path_manager,
                                       properties_settings, level_settings, allow_list_file, permissions_file);
}
