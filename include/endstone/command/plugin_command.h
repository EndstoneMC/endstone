// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <format>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "endstone/command/command.h"
#include "endstone/command/command_executor.h"
#include "endstone/plugin/plugin.h"
#include "endstone/util/pointers.h"

namespace endstone {

/**
 * Represents a Command belonging to a Plugin.
 */
class PluginCommand : public Command {
public:
    PluginCommand(const Command &command, const NotNull<Plugin> &owner) : Command(command), owner_(owner.get()) {}

    bool execute(const NotNull<CommandSender> &sender, const std::vector<std::string> &args) const override
    {
        const auto owner = getPlugin();
        if (!owner) {
            sender->sendMessage("Cannot execute command '{}' - plugin is disabled.", getName());
            return false;
        }
        if (!owner->isEnabled()) {
            sender->sendMessage("Cannot execute command '{}' in plugin {} - plugin is disabled.", getName(),
                                owner->getDescription().getFullName());
            return false;
        }

        if (!testPermission(sender)) {
            return true;
        }

        try {
            return getExecutor().onCommand(sender, *this, args);
        }
        catch (std::exception &e) {
            owner->getLogger().error("Unhandled exception executing command '{}' in plugin {}", getName(),
                                     owner->getDescription().getFullName());
            owner->getLogger().error(e.what());
            return false;
        }
    }

    /**
     * Sets the CommandExecutor to run when parsing this command.
     *
     * @param executor New executor to run, a null handle to fall back to the owning plugin
     */
    virtual void setExecutor(Nullable<CommandExecutor> executor) { executor_ = std::move(executor); }

    /**
     * Gets the CommandExecutor associated with this command.
     *
     * @return CommandExecutor object linked to this command
     */
    [[nodiscard]] virtual CommandExecutor &getExecutor() const
    {
        if (executor_) {
            return *executor_;
        }
        const auto owner = getPlugin();
        if (!owner) {
            throw std::runtime_error(std::format("Cannot execute command '{}' - plugin is disabled.", getName()));
        }
        return *owner;
    }

    /**
     * Gets the owner of this PluginCommand.
     *
     * @return Plugin that owns this command, or a null handle if the plugin has been unloaded
     */
    [[maybe_unused]] [[nodiscard]] Nullable<Plugin> getPlugin() const { return owner_.lock(); }

    [[nodiscard]] ClassInfo getClassInfo() const override { return typeid(PluginCommand); }
    [[nodiscard]] bool isInstanceOf(ClassInfo target) const override
    {
        return target == typeid(PluginCommand) || target == typeid(Command) || target == typeid(Object);
    }

private:
    std::weak_ptr<Plugin> owner_;
    Nullable<CommandExecutor> executor_;
};
}  // namespace endstone
