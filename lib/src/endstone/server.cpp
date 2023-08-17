//
// Created by Vincent on 31/07/2023.
//

#include "endstone/server.h"

Server::Server() : logger_(Logger::getLogger("Server"))
{
    try
    {
        py::gil_scoped_acquire lock{};
        auto module = py::module_::import("endstone.plugin");
        auto cls = module.attr("PluginManager");
        pluginManager_ = cls(this);
    }
    catch (const std::exception &e)
    {
        logger_.error("%s\n", e.what());
    }
}

void Server::loadPlugins()
{
    try
    {
        py::gil_scoped_acquire lock{};
        pluginManager_.attr("load_plugins")((std::filesystem::current_path() / "plugins").string());
    }
    catch (const std::exception &e)
    {
        logger_.error("%s\n", e.what());
    }
}

void Server::enablePlugins()
{
    try
    {
        py::gil_scoped_acquire lock{};
        pluginManager_.attr("enable_plugins")();
    }
    catch (const std::exception &e)
    {
        logger_.error("%s\n", e.what());
    }
}

void Server::disablePlugins()
{
    try
    {
        py::gil_scoped_acquire lock{};
        pluginManager_.attr("disable_plugins")();
    }
    catch (const std::exception &e)
    {
        logger_.error("%s\n", e.what());
    }
}

void Server::clearPlugins()
{
    try
    {
        py::gil_scoped_acquire lock{};
        pluginManager_.attr("clear_plugins")();
    }
    catch (const std::exception &e)
    {
        logger_.error("%s\n", e.what());
    }
}

void Server::start()
{
}

const Logger &Server::getLogger()
{
    return logger_;
}
