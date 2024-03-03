#include "bedrock/server/dedicated_server.h"

#include "endstone/detail/hook.h"
#include "endstone/detail/plugin/python_plugin_loader.h"
#include "endstone/detail/server.h"
#include "endstone/detail/singleton.h"

using endstone::detail::EndstoneServer;
using endstone::detail::PythonPluginLoader;
using endstone::detail::Singleton;

DedicatedServer::StartResult DedicatedServer::runDedicatedServerLoop(Core::FilePathManager &file_path_manager,
                                                                     PropertiesSettings &properties_settings,
                                                                     LevelSettings &level_settings,
                                                                     AllowListFile &allow_list_file,
                                                                     std::unique_ptr<PermissionsFile> &permissions_file)
{
    Singleton<EndstoneServer>::setInstance(std::make_unique<EndstoneServer>(*this));
    auto &server = Singleton<EndstoneServer>::getInstance();
    server.getPluginManager().registerLoader(std::make_unique<PythonPluginLoader>(server));
    server.getLogger().info("Version: {} (Minecraft: {})", server.getVersion(), server.getMinecraftVersion());
    return ENDSTONE_HOOK_CALL_ORIGINAL(&DedicatedServer::runDedicatedServerLoop, this, file_path_manager,
                                       properties_settings, level_settings, allow_list_file, permissions_file);
}
