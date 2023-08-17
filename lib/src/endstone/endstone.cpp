//
// Created by Vincent on 09/08/2023.
//

#include "endstone/endstone.h"
#include "common.h"

const std::string &Endstone::getVersion()
{
    static std::string version = []() -> std::string {
        py::gil_scoped_acquire lock{};
        auto module = py::module_::import("endstone");
        auto version = module.attr("__version__");
        return py::cast<std::string>(version);
    }();

    return version;
}
const ENDSTONE_API std::string &Endstone::getMinecraftVersion()
{
    static std::string version = []() -> std::string {
        py::gil_scoped_acquire lock{};
        auto module = py::module_::import("endstone");
        auto version = module.attr("__minecraft__version__");
        return py::cast<std::string>(version);
    }();

    return version;
}
