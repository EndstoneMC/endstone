//
// Created by Vincent on 09/08/2023.
//

#ifndef ENDSTONE_DEDICATED_SERVER_H
#define ENDSTONE_DEDICATED_SERVER_H

class DedicatedServer
{
  public:
    int runDedicatedServerLoop(void *file_path_manager, // Core::FilePathManager &
                               void *properties,        // PropertiesSettings &
                               void *level_settings,    // LevelSettings &
                               void *allow_list,        // AllowListFile &
                               void *permissions);      // std::unique_ptr<PermissionsFile> &

    void initializeLogging();
};

#endif // ENDSTONE_DEDICATED_SERVER_H
