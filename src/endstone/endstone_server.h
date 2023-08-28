//
// Created by Vincent on 28/08/2023.
//

#ifndef ENDSTONE_ENDSTONE_SERVER_H
#define ENDSTONE_ENDSTONE_SERVER_H

#include "endstone/server.h"

class EndstoneServer : public Server {
public:
    EndstoneServer();

    void loadPlugins() override;
    void enablePlugins() override;
    void disablePlugins() override;
    std::shared_ptr<Logger> getLogger() override;

    friend class CommandRegistry;
    friend class MinecraftCommands;

private:
    std::shared_ptr<Logger> logger_;
    std::shared_ptr<SimpleCommandMap> command_map_;
    std::unique_ptr<PluginManager> plugin_manager_;
};

#endif // ENDSTONE_ENDSTONE_SERVER_H
