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

#include "bedrock/deps/webrtc/port.h"

#include "endstone/runtime/hook.h"

namespace {
constexpr int StunErrorUnauthorized = 401;
}

void webrtc::Port::SendBindingErrorResponse(StunMessage *message, const SocketAddress &addr, int error_code,
                                            std::string_view reason)
{
    // Every port on a shared socket sees every binding request and answers 401 to the ones meant for a
    // sibling. That reply carries no message integrity, so the peer accepts it, drops the request it was
    // waiting on, and never matches the answer that does arrive.
    if (error_code == StunErrorUnauthorized) {
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&Port::SendBindingErrorResponse, this, message, addr, error_code, reason);
}
