//
// Created by Vincent on 28/08/2023.
//

#include "endstone_server.h"

#include "bedrock/command_registry.h"
#include "endstone/command/console_command_sender.h"
#include "endstone/command/plugin_command.h"
#include "endstone/plugin/cpp/cpp_plugin_loader.h"
#include "endstone/plugin/python/python_plugin_loader.h"
#include "endstone/plugin/simple_plugin_manager.h"
#include "logger_factory.h"

EndstoneServer::EndstoneServer()
    : logger_(LoggerFactory::getLogger("Server")), command_map_(std::make_unique<SimpleCommandMap>(*this)),
      plugin_manager_(std::make_unique<SimplePluginManager>(*this, *command_map_)),
      console_(std::make_unique<ConsoleCommandSender>())
{
}

void EndstoneServer::loadPlugins()
{
    try {
        plugin_manager_->registerLoader(
            std::make_unique<PythonPluginLoader>(*this, "endstone.plugin", "ZipPluginLoader"));
        plugin_manager_->registerLoader(
            std::make_unique<PythonPluginLoader>(*this, "endstone.plugin", "SourcePluginLoader"));
        plugin_manager_->registerLoader(std::make_unique<CppPluginLoader>(*this));

        auto pluginFolder = std::filesystem::current_path() / "plugins";

        if (exists(pluginFolder)) {
            auto plugins = plugin_manager_->loadPlugins(pluginFolder);
            for (const auto &plugin : plugins) {
                try {
                    plugin->getLogger()->info("Loading {}", plugin->getDescription().getFullName());
                    plugin->onLoad();
                }
                catch (std::exception &e) {
                    logger_->error("Error occurred when initializing {}: {}", plugin->getDescription().getFullName(),
                                   e.what());
                }
            }
        }
        else {
            create_directories(pluginFolder);
        }
    }
    catch (std::exception &e) {
        logger_->error("Error occurred when trying to load plugins: {}", e.what());
    }
}

void EndstoneServer::enablePlugins()
{
    auto plugins = plugin_manager_->getPlugins();
    for (const auto &plugin : plugins) {
        plugin_manager_->enablePlugin(*plugin);
    }

    command_map_->setFallbackCommands();
    setBedrockCommands();
    // TODO: permission
    // TODO: syncCommands
}

void EndstoneServer::disablePlugins()
{
    plugin_manager_->disablePlugins();
}

std::shared_ptr<Logger> EndstoneServer::getLogger()
{
    return logger_;
}

bool EndstoneServer::dispatchCommand(CommandSender &sender, const std::string &command_line)
{
    if (command_map_->dispatch(sender, command_line)) {
        return true;
    }

    if (dynamic_cast<ConsoleCommandSender *>(&sender)) {
        logger_->error("Unknown command. Type \"help\" for help.");
    }
    else {
        sender.sendMessage("Unknown command. Type \"/help\" for help.");
    }

    return false;
}
CommandSender &EndstoneServer::getConsoleSender()
{
    return *console_;
}

SimpleCommandMap &EndstoneServer::getCommandMap() const
{
    return *command_map_;
}

void EndstoneServer::setBedrockCommands()
{
    for (const auto &item : CommandRegistry::bedrock_commands) {
        command_map_->registerCommand("minecraft", item.second);
    }
}

std::shared_ptr<PluginCommand> EndstoneServer::getPluginCommand(const std::string &name)
{
    auto command = command_map_->getCommand(name);
    return std::dynamic_pointer_cast<PluginCommand>(command);
}
