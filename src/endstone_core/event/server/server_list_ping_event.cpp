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

#include "endstone/event/server/server_list_ping_event.h"

#include <sstream>

#include <fmt/format.h>
#include <magic_enum/magic_enum.hpp>

namespace endstone {

bool ServerListPingEvent::deserialize()
{
    try {
        std::istringstream iss(ping_response_);
        std::string tmp;
        std::getline(iss, tmp, ';');  // MCPE
        std::getline(iss, motd_, ';');
        std::getline(iss, tmp, ';');
        network_protocol_version_ = std::stoi(tmp);
        std::getline(iss, minecraft_version_network_, ';');
        std::getline(iss, tmp, ';');
        num_players_ = std::stoi(tmp);
        std::getline(iss, tmp, ';');
        max_players_ = std::stoi(tmp);
        std::getline(iss, server_guid_, ';');
        std::getline(iss, level_name_, ';');
        std::getline(iss, tmp, ';');
        auto game_mode = magic_enum::enum_cast<GameMode>(tmp);
        if (!game_mode.has_value()) {
            return false;
        }
        game_mode_ = game_mode.value();
        std::getline(iss, tmp, ';');  // unknown - 1
        std::getline(iss, tmp, ';');
        local_port_ = std::stoi(tmp);
        std::getline(iss, tmp, ';');
        local_port_v6_ = std::stoi(tmp);
        std::getline(iss, tmp, ';');  // unknown - 0
        return true;
    }
    catch (std::exception &e) {
        return false;
    }
}

std::string ServerListPingEvent::serialize()
{
    return fmt::format("MCPE;{};{};{};{};{};{};{};{};1;{};{};0;", motd_, network_protocol_version_,
                       minecraft_version_network_, num_players_, max_players_, server_guid_, level_name_,
                       magic_enum::enum_name(game_mode_), local_port_, local_port_v6_);
}

}  // namespace endstone
