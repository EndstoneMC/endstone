// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "endstone/chat_color.h"
#include "endstone/plugin/plugin.h"
#include "endstone/server.h"

class PluginCommand : public Command {
public:
    PluginCommand(const Command &command, Plugin &owner) noexcept : Command(command), owner_(owner) {}

public:
    bool execute(CommandSender &sender, const std::string &label, const std::vector<std::string> &args) const override
    {
        if (!owner_.isEnabled()) {
            throw std::runtime_error("Cannot execute command '" + label + "' in plugin " +
                                     owner_.getDescription().getFullName() + " - plugin is disabled.");
        }

        // TODO(permission): test sender's permission before execution
        // if (!testPermission(sender)) {
        //     return true;
        // }

        bool success;
        try {
            success = getExecutor().onCommand(sender, *this, label, args);
        }
        catch (std::exception &e) {
            throw std::runtime_error("Unhandled exception executing command '" + label + "' in plugin " +
                                     owner_.getDescription().getFullName() + ": " + e.what());
        }

        if (!success) {
            for (const auto &usage : usages_) {
                auto usage_msg = fmt::format(usage, fmt::arg("command", label));

                sender.sendMessage("Usage: {}", ChatColor::Red + usage_msg);
            }
        }

        return success;
    }

    /**
     * Sets the CommandExecutor to run when parsing this command
     *
     * @param executor New executor to run
     */
    void setExecutor(std::shared_ptr<CommandExecutor> executor) noexcept
    {
        executor_ = std::move(executor);
    }

    /**
     * Gets the {@link CommandExecutor} associated with this command
     *
     * @return CommandExecutor object linked to this command
     */
    [[nodiscard]] CommandExecutor &getExecutor() const noexcept
    {
        if (executor_) {
            return *executor_;
        }
        else {
            return owner_;
        }
    }

    /**
     * Gets the owner of this PluginCommand
     *
     * @return Plugin that owns this command
     */
    [[maybe_unused]] [[nodiscard]] Plugin &getPlugin() const noexcept
    {
        return owner_;
    }

private:
    Plugin &owner_;
    std::shared_ptr<CommandExecutor> executor_;
};
