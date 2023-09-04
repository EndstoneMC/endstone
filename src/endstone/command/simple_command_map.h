//
// Created by Vincent on 25/08/2023.
//

#ifndef ENDSTONE_SIMPLE_COMMAND_MAP_H
#define ENDSTONE_SIMPLE_COMMAND_MAP_H

#include "endstone/command/command_map.h"

class SimpleCommandMap : public CommandMap {

public:
    explicit SimpleCommandMap(Server &server);

    void registerAll(const std::string &fallback_prefix,
                     const std::vector<std::shared_ptr<Command>> &commands) noexcept override;
    bool registerCommand(std::string label, std::string fallback_prefix,
                         std::shared_ptr<Command> command) noexcept override;
    bool registerCommand(const std::string &fallback_prefix, std::shared_ptr<Command> command) noexcept override;
    bool dispatch(CommandSender &sender, const std::string &command_line) const override;
    void clearCommands() noexcept override;
    std::shared_ptr<Command> getCommand(std::string name) const noexcept override;

    void setFallbackCommands();
    std::vector<std::shared_ptr<Command>> getCommands() const noexcept;

protected:
    std::map<std::string, std::shared_ptr<Command>> known_commands_;

private:
    void setDefaultCommands();

    /**
     * Registers a command with the given name is possible. Also uses
     * fallbackPrefix to create a unique name.
     *
     * @param label the name of the command, without the '/'-prefix.
     * @param command the command to register
     * @param is_alias whether the command is an alias
     * @param fallback_prefix a prefix which is prepended to the command for a
     *     unique address
     * @return true if command was registered, false otherwise.
     */
    bool registerCommand(const std::string &label, const std::shared_ptr<Command> &command, bool is_alias,
                         const std::string &fallback_prefix) noexcept;

    Server &server_;
    std::mutex mutex_;
};

#endif // ENDSTONE_SIMPLE_COMMAND_MAP_H
