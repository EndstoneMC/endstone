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

#include "bedrock/network/packet/start_game_packet.h"

#include <entt/entt.hpp>

#include "endstone/detail/hook.h"
#include "endstone/detail/level/level.h"
#include "endstone/detail/server.h"

using endstone::detail::EndstoneLevel;
using endstone::detail::EndstoneServer;

void StartGamePacket::write(BinaryStream &stream) const
{
    static bool client_side_generation_enabled = []() {
        auto &server = entt::locator<EndstoneServer>::value();
        auto *level = static_cast<EndstoneLevel *>(server.getLevels()[0]);
        return level->getHandle().isClientSideGenerationEnabled();
    }();

    if (!client_side_generation_enabled) {
        settings_.setRandomSeed({0});
    }

#if _WIN32
    ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&StartGamePacket::write, __FUNCDNAME__, this, stream);
#else
    ENDSTONE_HOOK_CALL_ORIGINAL_NAME(&StartGamePacket::write, "_ZNK15StartGamePacket5writeER12BinaryStream", this,
                                     stream);
#endif
}
