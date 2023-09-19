// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "bedrock/dedicated_server.h"

#include "endstone/endstone_server.h"
#include "lib/hook/hook.h"

void DedicatedServer::initializeLogging()
{
    auto *filters = Endstone::Hook::sym_from_name<std::vector<std::string> *>("gPriorityFilters");
    if (std::find(filters->begin(), filters->end(), "VERBOSE") == filters->end()) {
        filters->emplace_back("VERBOSE");
    }

    ENDSTONE_HOOK_CALL_ORIGINAL(&DedicatedServer::initializeLogging, this)
}

DedicatedServer::StartResult DedicatedServer::runDedicatedServerLoop(Core::FilePathManager &file_path_manager,
                                                                     PropertiesSettings &properties_settings,
                                                                     LevelSettings &level_settings,
                                                                     AllowListFile &allow_list_file,
                                                                     std::unique_ptr<PermissionsFile> &permissions_file)
{
    auto &server = EndstoneServer::getInstance();
    server.getLogger().info("Endstone Version: {}", server.getVersion().c_str());
    return ENDSTONE_HOOK_CALL_ORIGINAL(&DedicatedServer::runDedicatedServerLoop, this, file_path_manager,
                                       properties_settings, level_settings, allow_list_file, permissions_file)
}
