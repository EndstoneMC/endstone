#include "bedrock/dedicated_server.h"

#include "endstone_core/endstone_server.h"
#include "endstone_runtime/internals.h"

DedicatedServer::StartResult DedicatedServer::runDedicatedServerLoop(Core::FilePathManager &file_path_manager,
                                                                     PropertiesSettings &properties_settings,
                                                                     LevelSettings &level_settings,
                                                                     AllowListFile &allow_list_file,
                                                                     std::unique_ptr<PermissionsFile> &permissions_file)
{
    auto &server = EndstoneServer::getInstance();
    server.getLogger().info("Version: {} (Minecraft: {})", server.getVersion(), server.getMinecraftVersion());
    return BEDROCK_CALL(&DedicatedServer::runDedicatedServerLoop, this, file_path_manager, properties_settings,
                        level_settings, allow_list_file, permissions_file);
}
