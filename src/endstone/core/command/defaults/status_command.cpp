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

#include "endstone/core/command/defaults/status_command.h"

#include <entt/entt.hpp>

#include "endstone/color_format.h"
#include "endstone/core/server.h"
#include "endstone/detail/platform.h"

namespace endstone::core {

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

    auto tps = server.getCurrentTicksPerSecond();
    auto avg_tps = server.getAverageTicksPerSecond();

    auto tps_color = ColorFormat::Green;
    if (tps < 12) {
        tps_color = ColorFormat::Red;
    }
    else if (tps < 17) {
        tps_color = ColorFormat::Gold;
    }

    auto avg_tps_color = ColorFormat::Green;
    if (avg_tps < 12) {
        avg_tps_color = ColorFormat::Red;
    }
    else if (avg_tps < 17) {
        avg_tps_color = ColorFormat::Gold;
    }

    sender.sendMessage("{}Current TPS: {}{:.2f} ({:.2f}%). {}MSPT: {}{:.2f}ms", ColorFormat::Gold, tps_color,
                       tps, server.getCurrentTickUsage() * 100, ColorFormat::Gold,
                       tps_color, server.getCurrentMillisecondsPerTick());
    sender.sendMessage("{}Average TPS: {}{:.2f} ({:.2f}%). {}MSPT: {}{:.2f}ms", ColorFormat::Gold, avg_tps_color,
                       avg_tps, server.getAverageTickUsage() * 100, ColorFormat::Gold,
                       avg_tps_color, server.getAverageMillisecondsPerTick());

    sender.sendMessage("{}Thread count: {}{}", ColorFormat::Gold, ColorFormat::Red, detail::get_thread_count());

    sender.sendMessage("{}Used memory: {}{:.2f} MB", ColorFormat::Gold, ColorFormat::Red,
                       detail::get_used_physical_memory() / 1024.0F / 1024.0F);
    sender.sendMessage("{}Total memory: {}{:.2f} MB", ColorFormat::Gold, ColorFormat::Red,
                       detail::get_total_virtual_memory() / 1024.0F / 1024.0F);

    auto *level = server.getLevel();
    sender.sendMessage("{}Level \"{}\":", ColorFormat::Gold, level->getName());
    auto actors = server.getLevel()->getActors();
    for (const auto &dimension : server.getLevel()->getDimensions()) {
        auto actor_count = 0;
        for (const auto &actor : actors) {
            if (&actor->getDimension() == dimension) {
                actor_count++;
            }
        }
        sender.sendMessage("- {}Dimension \"{}\": {}{}{} loaded chunks, {}{}{} entities",              //
                           ColorFormat::Gold, dimension->getName(),                                    //
                           ColorFormat::Red, dimension->getLoadedChunks().size(), ColorFormat::Green,  //
                           ColorFormat::Red, actor_count, ColorFormat::Green);
    }

    return true;
}

}  // namespace endstone::core
