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

#include <stdexcept>
#include <string>
#include <vector>

#include "endstone/command/command.h"
#include "endstone/command/command_executor.h"
#include "endstone/plugin/plugin.h"

namespace endstone {

/**
 * Represents a Command belonging to a Plugin.
 */
class PluginCommand : public Command {
public:
    PluginCommand(const Command &command, Plugin &owner) : Command(command), owner_(owner) {}

    bool execute(const NotNull<CommandSender> &sender, const std::vector<std::string> &args) const override
    {
        if (!owner_.isEnabled()) {
            sender->sendMessage("Cannot execute command '{}' in plugin {}. Plugin is disabled.", getName(),
                                getPlugin().getDescription().getFullName());
            return false;
        }

        if (!testPermission(sender)) {
            return true;
        }

        try {
            return getExecutor().onCommand(sender, *this, args);
        }
        catch (std::exception &e) {
            getPlugin().getLogger().error("Unhandled exception executing command '{}' in plugin {}", getName(),
                                          owner_.getDescription().getFullName());
            getPlugin().getLogger().error(e.what());
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
        return owner_;
    }

    /**
     * Gets the owner of this PluginCommand.
     *
     * @return Plugin that owns this command
     */
    [[maybe_unused]] [[nodiscard]] Plugin &getPlugin() const { return owner_; }

    [[nodiscard]] ClassInfo getClassInfo() const override { return typeid(PluginCommand); }
    [[nodiscard]] bool isInstanceOf(ClassInfo target) const override
    {
        return target == typeid(PluginCommand) || target == typeid(Command) || target == typeid(Object);
    }

private:
    Plugin &owner_;
    Nullable<CommandExecutor> executor_;
};
}  // namespace endstone
