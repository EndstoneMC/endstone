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

#include "bedrock/network/rak_peer_helper.h"

#include <entt/entt.hpp>

#include "endstone/core/server.h"
#include "endstone/runtime/hook.h"

RakNet::StartupResult RakPeerHelper::peerStartup(RakNet::RakPeerInterface *peer, const ConnectionDefinition &def,
                                                 PeerPurpose purpose)
{
    auto new_def = def;
    new_def.max_num_connections = SharedConstants::NetworkDefaultMaxConnections;

    return ENDSTONE_HOOK_CALL_ORIGINAL(&RakPeerHelper::peerStartup, this, peer, new_def, purpose);
}
