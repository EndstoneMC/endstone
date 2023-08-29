//
// Created by Vincent on 25/08/2023.
//

#ifndef ENDSTONE_COMMAND_MAP_H
#define ENDSTONE_COMMAND_MAP_H

#include "command.h"
#include "endstone/common.h"

class CommandMap {

public:
    virtual ~CommandMap() = default;

    /**
     * Registers all the commands belonging to a certain plugin.
     * <p>
     * Caller can use:
     * <ul>
     *  <li>command.getName() to determine the label registered for this
     *      command
     *  <li>command.getAliases() to determine the aliases which where
     *      registered
     * </ul>
     *
     * @param fallback_prefix a prefix which is prepended to each command with
     *     a ':' one or more times to make the command unique
     * @param commands a list of commands to register
     */
    virtual void registerAll(const std::string &fallback_prefix,
                             const std::vector<std::shared_ptr<Command>> &commands) noexcept = 0;

    /**
     * Registers a command. Returns true on success; false if name is already
     * taken and fallback had to be used.
     * <p>
     * Caller can use:
     * <ul>
     * <li>command.getName() to determine the label registered for this
     *     command
     * <li>command.getAliases() to determine the aliases which where
     *     registered
     * </ul>
     *
     * @param label the label of the command, without the '/'-prefix.
     * @param fallback_prefix a prefix which is prepended to the command with a
     *     ':' one or more times to make the command unique
     * @param command the command to register
     * @return true if command was registered with the passed in label, false
     *     otherwise, which indicates the fallbackPrefix was used one or more
     *     times
     */
    virtual bool registerCommand(std::string label, std::string fallback_prefix,
                                 std::shared_ptr<Command> command) noexcept = 0;

    /**
     * Registers a command. Returns true on success; false if name is already
     * taken and fallback had to be used.
     * <p>
     * Caller can use:
     * <ul>
     * <li>command.getName() to determine the label registered for this
     *     command
     * <li>command.getAliases() to determine the aliases which where
     *     registered
     * </ul>
     *
     * @param fallback_prefix a prefix which is prepended to the command with a
     *     ':' one or more times to make the command unique
     * @param command the command to register, from which label is determined
     *     from the command name
     * @return true if command was registered with the passed in label, false
     *     otherwise, which indicates the fallbackPrefix was used one or more
     *     times
     */
    virtual bool registerCommand(const std::string &fallback_prefix, std::shared_ptr<Command> command) noexcept = 0;

    /**
     * Looks for the requested command and executes it if found.
     *
     * @param sender The command's sender
     * @param command_line command + arguments. Example: "/test abc 123"
     * @return returns false if no target is found, true otherwise.
     * @throws std::exception Thrown when the executor for the given command fails with an unhandled exception
     */
    virtual bool dispatch(CommandSender &sender, const std::string &command_line) const = 0;

    /**
     * Clears all registered commands.
     */
    virtual void clearCommands() noexcept = 0;

    /**
     * Gets the command registered to the specified name
     *
     * @param name Name of the command to retrieve
     * @return Command with the specified name or nullptr if a command with that
     *     label doesn't exist
     */
    virtual std::shared_ptr<Command> getCommand(std::string name) const noexcept = 0;

    static std::string getCommandName(const std::string &command_line)
    {
        std::size_t start = command_line[0] == '/' ? 1 : 0;
        std::size_t end = command_line.find(' ');
        if (end == std::string::npos) {
            end = command_line.length();
        }

        return command_line.substr(start, end - start);
    }
};

#endif // ENDSTONE_COMMAND_MAP_H
