//
// Created by Vincent on 28/08/2023.
//

#ifndef ENDSTONE_ENDSTONE_SERVER_H
#define ENDSTONE_ENDSTONE_SERVER_H

#include "bedrock/minecraft_commands.h"
#include "endstone/command/simple_command_map.h"
#include "endstone/plugin/plugin_manager.h"
#include "endstone/server.h"

class EndstoneServer : public Server {
public:
    EndstoneServer();

    std::shared_ptr<Logger> getLogger() override;
    std::shared_ptr<PluginCommand> getPluginCommand(const std::string &name) override;
    bool dispatchCommand(CommandSender &sender, const std::string &command_line) override;
    CommandSender &getConsoleSender() override;

    void loadPlugins();
    void enablePlugins();
    void disablePlugins();
    SimpleCommandMap &getCommandMap() const;

private:
    void setBedrockCommands();

private:
    std::shared_ptr<Logger> logger_;
    std::unique_ptr<SimpleCommandMap> command_map_;
    std::unique_ptr<PluginManager> plugin_manager_;
    std::unique_ptr<CommandSender> console_;
};

#endif // ENDSTONE_ENDSTONE_SERVER_H
