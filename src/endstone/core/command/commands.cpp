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

#include "endstone/core/command/commands.h"

#include <algorithm>
#include <cctype>
#include <utility>

#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/algorithm/string/split.hpp>

#include "endstone/check.h"
#include "endstone/command/command_map.h"
#include "endstone/plugin/plugin.h"
#include "endstone/plugin/plugin_manager.h"
#include "endstone/server.h"

namespace endstone::core {

TreeCommand::TreeCommand(std::string owner, std::string name, std::string description, std::vector<std::string> aliases)
    : Command(std::move(name), std::move(description), {}, std::move(aliases)), owner_(std::move(owner))
{
}

bool TreeCommand::execute(const NotNull<CommandSender> &sender, const std::vector<std::string> &args) const
{
    const auto owner = sender->getServer().getPluginManager().getPlugin(owner_);
    if (!owner) {
        sender->sendMessage("Cannot execute command '{}' - plugin is disabled.", getName());
        return false;
    }
    if (!owner->isEnabled()) {
        sender->sendMessage("Cannot execute command '{}' in plugin {} - plugin is disabled.", getName(),
                            owner->getDescription().getFullName());
        return false;
    }

    auto command_line = getName();
    if (!args.empty()) {
        command_line += " " + boost::algorithm::join(args, " ");
    }
    return sender->getServer().getCommandMap().dispatch(sender, command_line);
}

const std::string &TreeCommand::getOwner() const
{
    return owner_;
}

ClassInfo TreeCommand::getClassInfo() const
{
    return typeid(TreeCommand);
}

bool TreeCommand::isInstanceOf(ClassInfo target) const
{
    return target == typeid(TreeCommand) || Command::isInstanceOf(target);
}

EndstoneCommands::EndstoneCommands(Server &server, std::string owner) : server_(server), owner_(std::move(owner)) {}

NotNull<SoftEnum> EndstoneCommands::registerSoftEnum(std::string name, std::vector<std::string> values)
{
    Preconditions::checkState(server_.isPrimaryThread(), "Soft enum '{}' must be registered on the server thread.",
                              name);
    return dynamic_cast<CommandRegistrar &>(server_.getCommandMap())
        .registerSoftEnum(owner_, std::move(name), std::move(values));
}

std::set<std::string> EndstoneCommands::registerCommand(
    const NotNull<brigadier::LiteralCommandNode<CommandSource>> &node, std::string description,
    std::vector<std::string> aliases)
{
    Preconditions::checkState(server_.isPrimaryThread(), "Command '{}' must be registered on the server thread.",
                              node->getLiteral());
    return dynamic_cast<CommandRegistrar &>(server_.getCommandMap())
        .registerTree(owner_, node, std::move(description), std::move(aliases));
}

std::set<std::string> EndstoneCommands::registerCommand(std::string label, std::string description,
                                                        std::vector<std::string> aliases, NotNull<BasicCommand> command)
{
    Preconditions::checkArgument(!label.empty() &&
                                     std::ranges::none_of(label, [](unsigned char c) { return std::isspace(c); }),
                                 "Command label '{}' is empty or holds whitespace.", label);
    const auto node =
        Commands::literal(label)
            .requires_([command](const CommandSource &source) { return command->canUse(source->getSender()); })
            .then(Commands::argument("args", brigadier::StringArgumentType::greedyString())
                      .executes([command](const brigadier::CommandContext<CommandSource> &context) {
                          std::vector<std::string> args;
                          boost::algorithm::split(args, context.getArgument<std::string>("args"),
                                                  boost::algorithm::is_any_of(" "));
                          std::erase(args, "");
                          command->execute(context.getSource(), args);
                          return 1;
                      }))
            .executes([command](const brigadier::CommandContext<CommandSource> &context) {
                command->execute(context.getSource(), {});
                return 1;
            })
            .build();
    return registerCommand(node, std::move(description), std::move(aliases));
}

}  // namespace endstone::core
