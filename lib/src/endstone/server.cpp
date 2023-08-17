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
    catch (py::error_already_set &e)
    {
        logger_.error("Failed to load PluginManager with Python exceptions: %s\n", e.what());
        throw e;
    }
    catch (const std::exception &e)
    {
        logger_.error("Failed to load PluginManager with Python C API exceptions: %s\n", e.what());
        throw e;
    }
}

void Server::loadPlugins()
{
    py::gil_scoped_acquire lock{};
    pluginManager_.attr("load_plugins")((std::filesystem::current_path() / "plugins").string());
}

void Server::enablePlugins()
{
    py::gil_scoped_acquire lock{};
    pluginManager_.attr("enable_plugins")();
}

void Server::disablePlugins()
{
    py::gil_scoped_acquire lock{};
    pluginManager_.attr("disable_plugins")();
}

void Server::clearPlugins()
{
    py::gil_scoped_acquire lock{};
    pluginManager_.attr("clear_plugins")();
}

void Server::start()
{
}

const Logger &Server::getLogger()
{
    return logger_;
}

