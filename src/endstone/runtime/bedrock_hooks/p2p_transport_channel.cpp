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

#include "bedrock/deps/webrtc/p2p_transport_channel.h"

#include "endstone/core/network/nethernet_address_cache.h"
#include "endstone/core/util/socket_address.h"
#include "endstone/runtime/hook.h"

void webrtc::P2PTransportChannel::SwitchSelectedConnectionInternal(Connection *conn, IceSwitchReason reason)
{
    // Read conn's live remote_candidate; the CandidatePairChangeEvent built below is a ToSanitizedCopy.
    if (conn != nullptr) {
        const auto &remote = reinterpret_cast<const CandidatePairInterface *>(conn)->remote_candidate();
        endstone::core::NetherNetAddressCache::getInstance().addAddress(
            remote, endstone::core::EndstoneSocketAddress::fromWebRtcCandidate(remote));
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&P2PTransportChannel::SwitchSelectedConnectionInternal, this, conn, reason);
}
