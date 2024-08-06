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

#include "endstone/detail/command/defaults/status_command.h"

#include <entt/entt.hpp>

#include "endstone/color_format.h"
#include "endstone/detail/server.h"

namespace endstone::detail {

StatusCommand::StatusCommand() : EndstoneCommand("status")
{
    setDescription("Gets the status of the server.");
    setUsages("/status");
    setPermissions("endstone.command.status");
}

bool StatusCommand::execute(CommandSender &sender, const std::vector<std::string> &args) const
{
    sender.sendMessage("{}---- {}Server status{} ----", ColorFormat::Green, ColorFormat::Reset, ColorFormat::Green);

    auto &server = entt::locator<EndstoneServer>::value();
    auto time =
        std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - server.getStartTime())
            .count();
    auto seconds = time % 60;
    auto minutes = (time / 60) % 60;
    auto hours = (time / (60 * 60)) % 24;
    auto days = time / (60 * 60 * 24);
    if (days > 0) {
        sender.sendMessage("{}Uptime: {}{} days {} hours {} minutes {} seconds", ColorFormat::Gold, ColorFormat::Red,
                           days, hours, minutes, seconds);
    }
    else if (hours > 0) {
        sender.sendMessage("{}Uptime: {}{} hours {} minutes {} seconds", ColorFormat::Gold, ColorFormat::Red, hours,
                           minutes, seconds);
    }
    else if (minutes > 0) {
        sender.sendMessage("{}Uptime: {}{} minutes {} seconds", ColorFormat::Gold, ColorFormat::Red, minutes, seconds);
    }
    else {
        sender.sendMessage("{}Uptime: {}{} seconds", ColorFormat::Gold, ColorFormat::Red, seconds);
    }

    auto color = ColorFormat::Green;
    auto average_tps = server.getAverageTicksPerSecond();
    if (average_tps < 12) {
        color = ColorFormat::Red;
    }
    else if (average_tps < 17) {
        color = ColorFormat::Gold;
    }

    sender.sendMessage("{}MSPT: {}{:.2f}", ColorFormat::Gold, color, server.getAverageMillisecondsPerTick());
    sender.sendMessage("{}TPS: {}{:.2f}", ColorFormat::Gold, color, server.getAverageTicksPerSecond());
    sender.sendMessage("{}Usage: {}{:.2f}%", ColorFormat::Gold, color, server.getAverageTickUsage() * 100);

    return true;
}

}  // namespace endstone::detail
