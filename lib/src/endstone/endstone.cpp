//
// Created by Vincent on 09/08/2023.
//

#include "endstone/endstone.h"

#include "endstone/common.h"
#include "pybind/pybind.h"

std::string Endstone::getVersion()
{
    static std::string version = []() -> std::string {
        py::gil_scoped_acquire lock{};
        auto module = py::module_::import("endstone");
        auto version = module.attr("__version__");
        return py::cast<std::string>(version);
    }();

    return version;
}

std::string Endstone::getMinecraftVersion()
{
    static std::string version = []() -> std::string {
        py::gil_scoped_acquire lock{};
        auto module = py::module_::import("endstone");
        auto version = module.attr("__minecraft__version__");
        return py::cast<std::string>(version);
    }();

    return version;
}

Server &Endstone::getServer()
{
    return *server_;
}

void Endstone::setServer(std::unique_ptr<Server> server)
{
    if (server_) {
        throw std::runtime_error("Server singleton is already set!");
    }

    server_ = std::move(server);
    server_->getLogger()->info("Endstone Version: {}", Endstone::getVersion().c_str());
}

std::unique_ptr<Server> Endstone::server_ = nullptr;
