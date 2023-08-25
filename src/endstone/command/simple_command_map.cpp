//
// Created by Vincent on 25/08/2023.
//

#include "simple_command_map.h"

#include "endstone_command.h"

SimpleCommandMap::SimpleCommandMap(Server &server) : server_(server) {}

void SimpleCommandMap::registerAll(const std::string &fallback_prefix,
                                   const std::vector<std::shared_ptr<Command>> &commands) noexcept
{
    for (auto &c : commands) {
        registerOne(fallback_prefix, c);
    }
}

bool SimpleCommandMap::registerOne(const std::string &fallback_prefix, std::shared_ptr<Command> command) noexcept
{
    return registerOne(command->getName(), fallback_prefix, command);
}

bool SimpleCommandMap::registerOne(std::string label, std::string fallback_prefix,
                                   std::shared_ptr<Command> command) noexcept
{
    std::transform(label.begin(), label.end(), label.begin(), [](auto c) {
        return std::tolower(c, std::locale("en_US.utf8"));
    });

    std::transform(fallback_prefix.begin(), fallback_prefix.end(), fallback_prefix.begin(), [](unsigned char c) {
        return std::tolower(c, std::locale("en_US.utf8"));
    });

    auto registered = registerOne(label, command, false, fallback_prefix);

    std::vector<std::string> aliases = command->getAliases();
    for (auto it = aliases.begin(); it != aliases.end();) {
        if (!registerOne(*it, command, true, fallback_prefix)) {
            it = aliases.erase(it);
        }
        else {
            ++it;
        }
    }

    // If we failed to register under the real name, we need to set the command label to the direct address
    if (!registered) {
        command->setLabel(fallback_prefix + ":" + label);
    }

    // Register to us so further updates of the commands label and aliases are postponed until it's re-registered
    command->registerCommand(shared_from_this());

    return registered;
}

bool SimpleCommandMap::registerOne(const std::string &label, const std::shared_ptr<Command> &command, bool is_alias,
                                   const std::string &fallback_prefix) noexcept
{
    std::lock_guard lock(mutex_);

    known_commands_[fallback_prefix + ":" + label] = command;

    if ((dynamic_cast<EndstoneCommand *>(command.get()) || is_alias) &&
        known_commands_.find(label) != known_commands_.end()) {
        // Request for an alias/fallback command which conflicts with an existing command or previous alias.
        // This will remove it from the list of active aliases
        return false;
    }

    bool registered = true;

    // If the command exists but is an alias we overwrite it, otherwise we return
    auto conflict = known_commands_[label];
    if (conflict && conflict->getLabel() == label) {
        return false;
    }

    if (!is_alias) {
        command->setLabel(label);
    }
    known_commands_[label] = command;

    return registered;
}

bool SimpleCommandMap::dispatch(CommandSender &sender, const std::string &command_line) const
{
    std::vector<std::string> args;
    std::size_t start = 0;
    std::size_t end = 0;
    while (start < command_line.length()) {
        end = command_line.find(' ', start);
        if (end == std::string::npos) {
            end = command_line.length();
        }

        args.push_back(command_line.substr(start, end - start));
        start = end + 1;
    }

    if (args.empty()) {
        return false;
    }

    auto label = args[0];
    std::transform(label.begin(), label.end(), label.begin(), [](auto c) {
        return std::tolower(c, std::locale("en_US.utf8"));
    });
    auto target = getCommand(label);

    if (!target) {
        return false;
    }

    try {
        target->execute(sender, label, std::vector<std::string>(args.begin() + 1, args.end()));
    }
    catch (std::exception &e) {
        throw std::runtime_error("Unhandled exception executing '" + command_line + "' in " + target->getName() + ": " +
                                 e.what());
    }

    return true;
}

void SimpleCommandMap::clearCommands() noexcept
{
    std::lock_guard lock(mutex_);
    for (const auto &item : known_commands_) {
        item.second->unregisterCommand(shared_from_this());
    }
    known_commands_.clear();
    setDefaultCommands();
}

std::shared_ptr<Command> SimpleCommandMap::getCommand(std::string name) const noexcept
{
    std::transform(name.begin(), name.end(), name.begin(), [](auto c) {
        return std::tolower(c, std::locale("en_US.utf8"));
    });
    return known_commands_.at(name);
}

void SimpleCommandMap::setDefaultCommands()
{
    // TODO: default commands...
}
