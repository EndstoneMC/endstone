//
// Created by Vincent on 28/08/2023.
//

#ifndef ENDSTONE_ENDSTONE_SERVER_H
#define ENDSTONE_ENDSTONE_SERVER_H

#include "bedrock/minecraft_commands.h"
#include "endstone/command/simple_command_map.h"
#include "endstone/server.h"

class EndstoneServer : public Server {
public:
    EndstoneServer();

    void loadPlugins() override;
    void enablePlugins() override;
    void disablePlugins() override;
    std::shared_ptr<Logger> getLogger() override;
    bool dispatchCommand(CommandSender &sender, const std::string &command_line) override;
    CommandSender &getConsoleSender() override;

    SimpleCommandMap &getCommandMap() const;

private:
    std::shared_ptr<Logger> logger_;
    std::shared_ptr<SimpleCommandMap> command_map_;
    std::unique_ptr<PluginManager> plugin_manager_;
    std::unique_ptr<CommandSender> console_;
};

#endif // ENDSTONE_ENDSTONE_SERVER_H
