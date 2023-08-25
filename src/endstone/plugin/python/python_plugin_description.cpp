//
// Created by Vincent on 22/08/2023.
//

#include "python_plugin_description.h"

#include "endstone/command/plugin_command.h"
#include "endstone/endstone.h"
#include "endstone/plugin/python/python_plugin.h"

PythonPluginDescription::PythonPluginDescription(py::object impl, PythonPlugin &owner)
    : impl_(std::move(impl)), owner_(owner)
{
}

PythonPluginDescription::~PythonPluginDescription()
{
    py::gil_scoped_acquire lock{};
    impl_.release();
}

std::string PythonPluginDescription::getName() const
{
    py::gil_scoped_acquire lock{};
    return impl_.attr("name").cast<std::string>();
}

std::string PythonPluginDescription::getVersion() const
{
    py::gil_scoped_acquire lock{};
    return impl_.attr("version").cast<std::string>();
}

std::optional<std::string> PythonPluginDescription::getDescription() const
{
    py::gil_scoped_acquire lock{};
    return impl_.attr("description").cast<std::optional<std::string>>();
}

std::optional<std::vector<std::string>> PythonPluginDescription::getAuthors() const
{
    py::gil_scoped_acquire lock{};
    return impl_.attr("authors").cast<std::optional<std::vector<std::string>>>();
}

std::optional<std::string> PythonPluginDescription::getPrefix() const
{
    py::gil_scoped_acquire lock{};
    return impl_.attr("prefix").cast<std::optional<std::string>>();
}

std::string PythonPluginDescription::getFullName() const
{
    py::gil_scoped_acquire lock{};
    return impl_.attr("fullname").cast<std::string>();
}

std::vector<std::shared_ptr<Command>> PythonPluginDescription::getCommands() const
{
    py::gil_scoped_acquire lock{};
    std::vector<std::shared_ptr<Command>> commands;

    if (!hasattr(impl_, "commands")) {
        return commands;
    }

    auto logger = Endstone::getServer().getLogger();

    auto map = impl_.attr("commands").cast<py::dict>();
    for (auto item : map) {
        auto name = item.first.cast<std::string>();
        auto value = item.second.cast<py::dict>();

        if (name.find(":") != std::string::npos) {
            logger->error("Could not load command {} for plugin {}: Illegal Characters", name, getName());
            continue;
        }

        auto command = std::shared_ptr<PluginCommand>(new PluginCommand(name, owner_));
        auto description = value.attr("get")("description", py::none());
        auto usage = value.attr("get")("usage", py::none());
        auto aliases = value.attr("get")("aliases", py::none());

        // TODO #permissions:
        //        auto permission = value.attr("get")("permission", py::none());
        //        auto permission_message = value.attr("get")("permission-message", py::none());

        if (!description.is_none()) {
            command->setDescription(description.cast<std::string>());
        }

        if (!usage.is_none()) {
            command->setUsage(usage.cast<std::string>());
        }

        if (!aliases.is_none()) {
            std::vector<std::string> alias_list;

            if (py::isinstance<py::list>(aliases)) {
                for (auto o : aliases.cast<py::list>()) {
                    auto alias = o.cast<std::string>();
                    if (alias.find(":") != std::string::npos) {
                        logger->error("Could not load alias {} for plugin {}: Illegal Characters", alias, getName());
                        continue;
                    }
                    alias_list.push_back(alias);
                }
            }
            else {
                auto alias = aliases.cast<std::string>();
                if (alias.find(":") != std::string::npos) {
                    logger->error("Could not load alias {} for plugin {}: Illegal Characters", alias, getName());
                    continue;
                }
                alias_list.push_back(alias);
            }

            command->setAliases(alias_list);
        }

        // TODO: permission system
        //        if (permission != null) {
        //            command.setPermission(permission.cast<std::string>());
        //        }
        //        if (permissionMessage != null) {
        //            command.setPermissionMessage(permission_message.cast<std::string>());
        //        }

        commands.push_back(command);
    }

    return commands;
}
