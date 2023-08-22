//
// Created by Vincent on 17/08/2023.
//

#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_loader.h"
#include "endstone/plugin/plugin_logger.h"
#include "endstone/pybind.h"

class PyPlugin : public Plugin
{
  public:
    const PluginDescription &getDescription() const override
    {
        PYBIND11_OVERRIDE_PURE_NAME(const PluginDescription &, Plugin, "get_description", getDescription);
    }

    void onLoad() override
    {
        PYBIND11_OVERRIDE_PURE_NAME(void, Plugin, "on_load", onLoad);
    }

    void onEnable() override
    {
        PYBIND11_OVERRIDE_PURE_NAME(void, Plugin, "on_enable", onEnable);
    }

    void onDisable() override
    {
        PYBIND11_OVERRIDE_PURE_NAME(void, Plugin, "on_disable", onDisable);
    }

    std::shared_ptr<Logger> getLogger() const override
    {
        PYBIND11_OVERRIDE_PURE_NAME(std::shared_ptr<Logger>, Plugin, "get_logger", onDisable);
    }

    bool isEnabled() const override
    {
        PYBIND11_OVERRIDE_PURE_NAME(bool, Plugin, "is_enabled", isEnabled);
    }

    std::shared_ptr<const PluginLoader> getPluginLoader() const override
    {
        throw std::runtime_error("Not supported");
    }
};

class PyPluginDescription : public PluginDescription
{
  public:
    using PluginDescription::PluginDescription;

    std::string getName() const override
    {
        PYBIND11_OVERRIDE_PURE_NAME(std::string, PluginDescription, "get_name", getName);
    }

    std::string getVersion() const override
    {
        PYBIND11_OVERRIDE_PURE_NAME(std::string, PluginDescription, "get_version", getVersion);
    }

    std::optional<std::string> getDescription() const override
    {
        PYBIND11_OVERRIDE_PURE_NAME(std::optional<std::string>, PluginDescription, "get_description", getDescription);
    }

    std::optional<std::vector<std::string>> getAuthors() const override
    {
        PYBIND11_OVERRIDE_PURE_NAME(
            std::optional<std::vector<std::string>>, PluginDescription, "get_authors", getAuthors);
    }

    std::optional<std::string> getPrefix() const override
    {
        PYBIND11_OVERRIDE_PURE_NAME(std::optional<std::string>, PluginDescription, "get_prefix", getPrefix);
    }

    std::string getFullName() const override
    {
        return getName() + " v" + getVersion();
    }
};

class PyPluginLoader : public PluginLoader
{
  public:
    Plugin *loadPlugin(const std::string &file) const override
    {
        PYBIND11_OVERRIDE_PURE_NAME(Plugin *, PluginLoader, "load_plugin", loadPlugin);
    }

    std::vector<std::string> getPluginFilters() const noexcept override
    {
        PYBIND11_OVERRIDE_PURE_NAME(std::vector<std::string>, PluginLoader, "get_plugin_filters", getPluginFilters);
    }

    void enablePlugin(Plugin &plugin) const override
    {
        PYBIND11_OVERRIDE_PURE_NAME(void, PluginLoader, "enable_plugin", enablePlugin);
    }

    void disablePlugin(Plugin &plugin) const override
    {
        PYBIND11_OVERRIDE_PURE_NAME(void, PluginLoader, "disable_plugin", disablePlugin);
    }
};

PYBIND11_MODULE(_plugin, m)
{
    py::class_<Plugin, PyPlugin>(m, "IPlugin") //
        .def(py::init<>())                     //
        .def("get_description", &Plugin::getDescription, py::return_value_policy::reference_internal)
        .def("on_load", &Plugin::onLoad)       //
        .def("on_enable", &Plugin::onEnable)   //
        .def("on_disable", &Plugin::onDisable) //
        .def("is_enabled", &Plugin::isEnabled) //
        .def("get_logger", &Plugin::getLogger, py::return_value_policy::reference_internal);

    py::class_<PluginDescription, PyPluginDescription>(m, "IPluginDescription") //
        .def(py::init<>())                                                      //
        .def("get_name", &PluginDescription::getName)                           //
        .def("get_version", &PluginDescription::getVersion)                     //
        .def("get_description", &PluginDescription::getDescription)             //
        .def("get_authors", &PluginDescription::getAuthors)                     //
        .def("get_prefix", &PluginDescription::getPrefix)                       //
        .def("get_fullname", &PluginDescription::getFullName);

    py::class_<PluginLoader, PyPluginLoader>(m, "IPluginLoader")              //
        .def(py::init<>())                                                    //
        .def("load_plugin", &PluginLoader::loadPlugin, py::arg("file"))       //
        .def("get_plugin_filters", &PluginLoader::getPluginFilters)           //
        .def("enable_plugin", &PluginLoader::enablePlugin, py::arg("plugin")) //
        .def("disable_plugin", &PluginLoader::disablePlugin, py::arg("plugin"));

    py::class_<PluginLogger>(m, "PluginLogger")
        .def(py::init<const Plugin &>())
        .def(
            "log",
            [](const PluginLogger &logger, const LogLevel level, const std::string &msg) {
                return logger.log(level, msg);
            },
            py::arg("level"),
            py::arg("msg"))
        .def(
            "verbose",
            [](const PluginLogger &logger, const std::string &msg) {
                return logger.log(LogLevel::Verbose, msg);
            },
            py::arg("msg"))
        .def(
            "info",
            [](const PluginLogger &logger, const std::string &msg) {
                return logger.log(LogLevel::Info, msg);
            },
            py::arg("msg"))
        .def(
            "warning",
            [](const PluginLogger &logger, const std::string &msg) {
                return logger.log(LogLevel::Warning, msg);
            },
            py::arg("msg"))
        .def(
            "error",
            [](const PluginLogger &logger, const std::string &msg) {
                return logger.log(LogLevel::Error, msg);
            },
            py::arg("msg"))
        .def("set_level", &PluginLogger::setLevel, py::arg("level"))
        .def("get_name", &PluginLogger::getName);
}
