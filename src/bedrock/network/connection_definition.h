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

#include <cstdint>

struct ConnectionDefinition {
    enum class PortBusyFallbackPolicy : int {  // size=0x4
        UseRandom = 0,
        Fail = 0x1,
    };

    std::uint16_t ipv4_port;                  // +0
    std::uint16_t ipv6_port;                  // +2
    PortBusyFallbackPolicy fallback;          // +4
    std::uint32_t max_num_players;            // +8
    std::uint32_t max_num_connections;        // +12
    bool is_server_visible_to_lan_discovery;  // +16
    bool allow_unconnected_pongs;             // +17
    bool disable_trickle_ice;
};
