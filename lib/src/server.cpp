//
// Created by Vincent on 31/07/2023.
//

#include "server.h"
#include "logger.h"
#include "plugin.h"

Server::Server() : pluginManager(std::make_unique<PluginManager>(*this))
{
}

PluginManager &Server::getPluginManager() const
{
    return *pluginManager;
}