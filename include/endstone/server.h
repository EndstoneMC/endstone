//
// Created by Vincent on 31/07/2023.
//

#ifndef ENDSTONE_SERVER_H
#define ENDSTONE_SERVER_H

#include "common.h"
#include "logger.h"

class PluginCommand;
class CommandSender;

class Server {
public:
    Server(const Server &) = delete;            // Disable copy constructor
    Server &operator=(const Server &) = delete; // Disable assignment operator

    Server() = default;
    virtual ~Server() = default;

    /**
     * Returns the primary logger associated with this server instance.
     *
     * @return Logger associated with this server
     */
    virtual std::shared_ptr<Logger> getLogger() = 0;

    /**
     * Gets a PluginCommand with the given name or alias.
     *
     * @param name the name of the command to retrieve
     * @return a plugin command if found, null otherwise
     */
    virtual std::shared_ptr<PluginCommand> getPluginCommand(const std::string &name) = 0;

    /**
     * Dispatches a command on this server, and executes it if found.
     *
     * @param sender the apparent sender of the command
     * @param commandLine the command + arguments. Example: <code>test abc 123</code>
     * @return returns false if no target is found
     * @throws std::exception thrown when the executor for the given command
     *     fails with an unhandled exception
     */
    virtual bool dispatchCommand(CommandSender &sender, const std::string &command_line) = 0;

    /**
     * Gets a ConsoleCommandSender that may be used as an input source
     * for this server.
     *
     * @return a console command sender
     */
    virtual CommandSender &getConsoleSender() = 0;
};

#endif // ENDSTONE_SERVER_H
