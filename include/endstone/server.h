//
// Created by Vincent on 31/07/2023.
//

#ifndef ENDSTONE_SERVER_H
#define ENDSTONE_SERVER_H

#include "common.h"
#include "endstone/command/simple_command_map.h"
#include "logger.h"
#include "plugin/plugin_manager.h"

class Server {
public:
    Server(const Server &) = delete;            // Disable copy constructor
    Server &operator=(const Server &) = delete; // Disable assignment operator

    Server() = default;
    virtual ~Server() = default;

    virtual void loadPlugins() = 0;
    virtual void enablePlugins() = 0;
    virtual void disablePlugins() = 0;
    virtual std::shared_ptr<Logger> getLogger() = 0;
    virtual bool dispatchCommand(CommandSender &sender, const std::string &command_line) = 0;
    virtual CommandSender &getConsoleSender() = 0;
};

#endif // ENDSTONE_SERVER_H
