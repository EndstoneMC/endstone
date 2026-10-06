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

#include "bedrock/bedrock.h"
#include "bedrock/deps/webrtc/candidate.h"

namespace webrtc {

class Connection;

enum class IceSwitchReason : int {
    UNKNOWN = 0,
    REMOTE_CANDIDATE_GENERATION_CHANGE = 1,
    NETWORK_PREFERENCE_CHANGE = 2,
    NEW_CONNECTION_FROM_LOCAL_CANDIDATE = 3,
    NEW_CONNECTION_FROM_REMOTE_CANDIDATE = 4,
    NEW_CONNECTION_FROM_UNKNOWN_REMOTE_ADDRESS = 5,
    NOMINATION_ON_CONTROLLED_SIDE = 6,
    DATA_RECEIVED = 7,
    CONNECT_STATE_CHANGE = 8,
    SELECTED_CONNECTION_DESTROYED = 9,
    ICE_CONTROLLER_RECHECK = 10,
    APPLICATION_REQUESTED = 11,
};

class P2PTransportChannel {
private:
    ENDSTONE_HOOK void SwitchSelectedConnectionInternal(Connection *conn, IceSwitchReason reason);
};

}  // namespace webrtc
