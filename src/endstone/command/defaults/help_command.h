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

#include <string>
#include <vector>

#include "endstone/command/endstone_command.h"
#include "endstone/command/simple_command_map.h"

class HelpCommand : public EndstoneCommand {
private:
    constexpr static int NumCommandPerPage = 8;

public:
    explicit HelpCommand(const SimpleCommandMap &command_map) noexcept;
    bool execute(CommandSender &sender, const std::string &label,
                 const std::vector<std::string> &args) const noexcept override;

private:
    void displayHelpPage(const CommandSender &sender, int page) const noexcept;
    void displayHelpCommand(const CommandSender &sender, const std::string &name) const noexcept;

    const SimpleCommandMap &command_map_;
};
