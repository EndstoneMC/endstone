//
// Created by Vincent on 25/08/2023.
//

#ifndef ENDSTONE_PLUGIN_COMMAND_H
#define ENDSTONE_PLUGIN_COMMAND_H

#include "command.h"
#include "endstone/command/console_command_sender.h"
#include "endstone/plugin/plugin.h"
#include "endstone/server.h"

class PluginCommand : public Command {

public:
    PluginCommand(const Command &command, Plugin &owner) noexcept : Command(command), owner_(owner){};

public:
    bool execute(const CommandSender &sender, const std::string &label,
                 const std::vector<std::string> &args) const override
    {
        if (!owner_.isEnabled()) {
            throw std::runtime_error("Cannot execute command '" + label + "' in plugin " +
                                     owner_.getDescription().getFullName() + " - plugin is disabled.");
        }

        // TODO: permission check
        //  if (!testPermission(sender)) {
        //      return true;
        //  }

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

                if (dynamic_cast<const ConsoleCommandSender *>(&sender)) {
                    sender.getServer().getLogger()->error("Usage: {}", usage_msg);
                }
                else {
                    sender.sendMessage("Usage: {}", usage_msg);
                }
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
        executor_ = executor;
    }

    /**
     * Gets the {@link CommandExecutor} associated with this command
     *
     * @return CommandExecutor object linked to this command
     */
    CommandExecutor &getExecutor() const noexcept
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
    Plugin &getPlugin() const noexcept
    {
        return owner_;
    }

private:
    Plugin &owner_;
    std::shared_ptr<CommandExecutor> executor_;
};

#endif // ENDSTONE_PLUGIN_COMMAND_H
