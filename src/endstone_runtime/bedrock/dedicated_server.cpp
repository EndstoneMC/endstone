#include "bedrock/server/dedicated_server.h"

#include "endstone/detail/hook.h"
#include "endstone/detail/server.h"

DedicatedServer::StartResult DedicatedServer::runDedicatedServerLoop(Core::FilePathManager &file_path_manager,
                                                                     PropertiesSettings &properties_settings,
                                                                     LevelSettings &level_settings,
                                                                     AllowListFile &allow_list_file,
                                                                     std::unique_ptr<PermissionsFile> &permissions_file)
{
    auto &server = endstone::detail::EndstoneServer::getInstance();
    server.getLogger().info("Version: {} (Minecraft: {})", server.getVersion(), server.getMinecraftVersion());
    return ENDSTONE_HOOK_CALL_ORIGINAL(&DedicatedServer::runDedicatedServerLoop, this, file_path_manager,
                                       properties_settings, level_settings, allow_list_file, permissions_file);
}
