//
// Created by Vincent on 24/08/2023.
//

#ifndef ENDSTONE_COMMAND_H
#define ENDSTONE_COMMAND_H

#include "command_sender.h"
#include "endstone/common.h"

class CommandMap;

/**
 * Represents a Command, which executes various tasks upon user input
 */
class Command {

public:
    explicit Command(const std::string &name) : Command(name, "", {"/" + name}, {}){};
    explicit Command(const std::string &name, const std::string &description, const std::vector<std::string> &usages,
                     const std::vector<std::string> &aliases)
        : name_(name), label_(name), next_label_(name), description_(description), usages_(usages), aliases_(aliases),
          active_aliases_(std::vector<std::string>(aliases)){};
    virtual ~Command() = default;

protected:
    Command(const Command &command) = default;

public:
    /**
     * Executes the command, returning its success
     *
     * @param sender Source object which is executing this command
     * @param label The alias of the command used
     * @param args All arguments passed to the command, split via ' '
     * @return true if the command was successful, otherwise false
     */
    virtual bool execute(const CommandSender &sender, const std::string &label,
                         const std::vector<std::string> &args) const
    {
        throw std::logic_error("Command::execute() is not implemented.");
    }

    /**
     * Returns the name of this command
     *
     * @return Name of this command
     */
    const std::string &getName() const noexcept
    {
        return name_;
    }

    /**
     * Sets the name of this command.
     * <p>
     * May only be used before registering the command.
     * Will return true if the new name is set, and false
     * if the command has already been registered.
     *
     * @param name New command name
     * @return returns true if the name change happened instantly or false if
     *     the command was already registered
     */
    bool setName(const std::string &name) noexcept
    {
        if (!isRegistered()) {
            name_ = name;
            return true;
        }

        return false;
    }

    /**
     * Returns the label for this command
     *
     * @return Label of this command
     */
    const std::string &getLabel() const noexcept
    {
        return label_;
    }

    /**
     * Sets the label of this command.
     * <p>
     * May only be used before registering the command.
     * Will return true if the new name is set, and false
     * if the command has already been registered.
     *
     * @param name The command's name
     * @return returns true if the name change happened instantly or false if
     *     the command was already registered
     */
    bool setLabel(const std::string &name) noexcept
    {
        next_label_ = name;

        if (!isRegistered()) {
            label_ = name;
            return true;
        }

        return false;
    }

    /**
     * Registers this command to a CommandMap.
     * Once called it only allows changes the registered CommandMap
     *
     * @param command_map the CommandMap to register this command to
     * @return true if the registration was successful (the current registered
     *     CommandMap was the passed CommandMap or null) false otherwise
     */
    bool registerTo(CommandMap &command_map) noexcept
    {
        if (allowChangesFrom(command_map)) {
            command_map_ = &command_map;
            return true;
        }

        return false;
    }

    /**
     * Unregisters this command from the passed CommandMap applying any
     * outstanding changes
     *
     * @param command_map the CommandMap to unregister
     * @return true if the unregistration was successful (the current
     *     registered CommandMap was the passed CommandMap or null) false
     *     otherwise
     */
    bool unregisterFrom(CommandMap &command_map) noexcept
    {
        if (allowChangesFrom(command_map)) {
            command_map_ = nullptr;
            active_aliases_ = std::vector<std::string>(aliases_);
            label_ = next_label_;
            return true;
        }

        return false;
    }

    /**
     * Returns the current registered state of this command
     *
     * @return true if this command is currently registered false otherwise
     */
    bool isRegistered() const noexcept
    {
        return command_map_;
    }

    /**
     * Returns a list of aliases of this command
     *
     * @return List of aliases
     */
    const std::vector<std::string> &getAliases() const
    {
        return active_aliases_;
    }

    /**
     * Gets a brief description of this command
     *
     * @return Description of this command
     */
    const std::string &getDescription() const
    {
        return description_;
    }

    /**
     * Gets example usages of this command
     *
     * @return List of example usages
     */
    const std::vector<std::string> &getUsages() const
    {
        return usages_;
    }

    /**
     * Sets the list of aliases to request on registration for this command.
     *
     * @param aliases aliases to register to this command
     * @return this command object, for chaining
     */
    Command &setAliases(const std::vector<std::string> &aliases)
    {
        aliases_ = aliases;
        if (!isRegistered()) {
            active_aliases_ = std::vector<std::string>(aliases);
        }

        return *this;
    }

    /**
     * Sets a brief description of this command.
     *
     * @param description new command description
     * @return this command object, for chaining
     */
    Command &setDescription(const std::string &description)
    {
        description_ = description;
        return *this;
    }

    /**
     * Sets the example usage of this command
     *
     * @param usages new example usage
     * @return this command object, for chaining
     */
    Command &setUsages(std::vector<std::string> usages)
    {
        usages_ = std::move(usages);
        return *this;
    }

private:
    bool allowChangesFrom(CommandMap &command_map)
    {
        return (!isRegistered() || command_map_ == &command_map);
    }

private:
    std::string name_;
    std::string label_;
    std::string next_label_;
    std::vector<std::string> aliases_;
    std::vector<std::string> active_aliases_;

protected:
    std::vector<std::string> usages_;
    std::string description_;

private:
    CommandMap *command_map_{nullptr};
};

#endif // ENDSTONE_COMMAND_H
