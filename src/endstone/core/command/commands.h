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

#include <set>
#include <string>
#include <vector>

#include "endstone/command/brigadier/commands.h"
#include "endstone/command/command.h"

namespace endstone {
class Server;
namespace core {

class CommandRegistrar {
public:
    virtual ~CommandRegistrar() = default;
    virtual NotNull<SoftEnum> registerSoftEnum(std::string owner, std::string name,
                                               std::vector<std::string> values) = 0;
    virtual std::set<std::string> registerTree(std::string owner,
                                               const NotNull<brigadier::LiteralCommandNode<CommandSource>> &node,
                                               std::string description, std::vector<std::string> aliases) = 0;
};

class TreeCommand : public Command {
public:
    TreeCommand(std::string owner, std::string name, std::string description, std::vector<std::string> aliases);
    [[nodiscard]] bool execute(const NotNull<CommandSender> &sender,
                               const std::vector<std::string> &args) const override;
    [[nodiscard]] const std::string &getOwner() const;
    [[nodiscard]] ClassInfo getClassInfo() const override;
    [[nodiscard]] bool isInstanceOf(ClassInfo target) const override;

private:
    std::string owner_;
};

class EndstoneCommands : public Commands {
public:
    EndstoneCommands(Server &server, std::string owner);
    NotNull<SoftEnum> registerSoftEnum(std::string name, std::vector<std::string> values) override;
    std::set<std::string> registerCommand(const NotNull<brigadier::LiteralCommandNode<CommandSource>> &node,
                                          std::string description, std::vector<std::string> aliases) override;
    std::set<std::string> registerCommand(std::string label, std::string description, std::vector<std::string> aliases,
                                          NotNull<BasicCommand> command) override;

private:
    Server &server_;
    std::string owner_;
};

}  // namespace core
}  // namespace endstone
