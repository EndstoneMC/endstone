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
#include <string>

#include "bedrock/certificates/identity/player_authentication_info.h"
#include "bedrock/core/utility/binary_stream.h"
#include "bedrock/network/connection_type.h"
#include "bedrock/network/nonce.h"
#include "bedrock/world/level/game_type.h"

class NetherNetServerLocator {
public:
    struct ServerData {
        static constexpr std::uint8_t VERSION_NUMBER = 7;
        std::uint8_t version;
        std::string server_name;
        std::int32_t protocol_version;
        std::string app_version;
        std::string level_name;
        GameType game_type;
        std::int32_t num_players;
        std::int32_t max_num_players;
        bool is_editor_world;
        bool is_hardcore;
        ServerSupportedAuthenticationTypes supported_auth;
        Social::Nonce nonce;
        Social::ConnectionType connection_type;

        void write(BinaryStream &stream) const;                 // Endstone
        Bedrock::Result<void> read(ReadOnlyBinaryStream &stream);  // Endstone
    };
};
