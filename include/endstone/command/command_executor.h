//
// Created by Vincent on 25/08/2023.
//

#ifndef ENDSTONE_COMMAND_EXECUTOR_H
#define ENDSTONE_COMMAND_EXECUTOR_H

#include "command.h"
#include "command_sender.h"

/**
 * Represents a class which contains a single method for executing commands
 */
class CommandExecutor {

public:
    virtual ~CommandExecutor() = default;

    /**
     * Executes the given command, returning its success.
     *
     * If false is returned, then the command usages
     * (if defined) will be sent to the player.
     *
     * @param sender Source of the command
     * @param command Command which was executed
     * @param label Alias of the command which was used
     * @param args Passed command arguments
     * @return true if a valid command, otherwise false
     */
    virtual bool onCommand(const CommandSender &sender, const Command &command, const std::string &label,
                           const std::vector<std::string> &args) noexcept = 0;
};

#endif // ENDSTONE_COMMAND_EXECUTOR_H
