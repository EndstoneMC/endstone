//
// Created by Vincent on 09/08/2023.
//

#ifndef ENDSTONE_DEDICATED_SERVER_H
#define ENDSTONE_DEDICATED_SERVER_H

#include "bedrock_common.h"

class DedicatedServer {
public:
    enum class StartResult;

public:
    BEDROCK_API DedicatedServer::StartResult runDedicatedServerLoop(
        Core::FilePathManager &file_path_manager, class PropertiesSettings &properties_settings,
        class LevelSettings &level_settings, class AllowListFile &allow_list_file,
        class std::unique_ptr<class PermissionsFile> &permissions_file);

private:
    BEDROCK_API void initializeLogging();
};

#endif // ENDSTONE_DEDICATED_SERVER_H
