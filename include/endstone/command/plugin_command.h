//
// Created by Vincent on 25/08/2023.
//

#ifndef ENDSTONE_PLUGIN_COMMAND_H
#define ENDSTONE_PLUGIN_COMMAND_H

#include "command.h"
#include "endstone/plugin/plugin.h"

class CommandExecutor;

class PluginCommand : public Command {
public:
    friend class PythonPluginDescription;
    friend class CppPluginDescription;

protected:
    PluginCommand(const std::string &name, Plugin &owner) noexcept : Command(name), owner_(owner)
    {
        usage_ = "";
    }

public:
    bool execute(CommandSender &sender, const std::string &label, const std::vector<std::string> &args) const override
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
            success = executor_->onCommand(sender, *this, label, std::forward<const std::vector<std::string> &>(args));
        }
        catch (std::exception &e) {
            throw std::runtime_error("Unhandled exception executing command '" + label + "' in plugin " +
                                     owner_.getDescription().getFullName() + ": " + e.what());
        }

        if (!success && !usage_.empty()) {
            size_t position = 0;
            std::string line;
            constexpr std::string placeholder = "<command>";
            while (position < usage_.length()) {
                // If we find the placeholder
                if (usage_.compare(position, placeholder.length(), placeholder) == 0) {
                    line += label;
                    position += placeholder.length(); // Skip past placeholder
                    continue;
                }

                // If it's a newline, send the accumulated line and reset it
                if (usage_[position] == '\n') {
                    sender.sendMessage(line);
                    line.clear();
                    ++position; // Skip past newline
                    continue;
                }

                line += usage_[position];
                ++position;
            }

            // Handle the remaining line, if any
            if (!line.empty()) {
                sender.sendMessage(line);
            }
        }

        return success;
    }

    /**
     * Sets the CommandExecutor to run when parsing this command
     *
     * @param executor New executor to run
     */
    void setExecutor(std::unique_ptr<CommandExecutor> &executor) noexcept
    {
        if (executor) {
            executor_ = std::move(executor);
        }
        else {
            executor_ = std::unique_ptr<CommandExecutor>(&owner_);
        }
    }

    /**
     * Gets the {@link CommandExecutor} associated with this command
     *
     * @return CommandExecutor object linked to this command
     */
    CommandExecutor &getExecutor() const noexcept
    {
        return *executor_;
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
    std::unique_ptr<CommandExecutor> executor_;
};

#endif // ENDSTONE_PLUGIN_COMMAND_H
