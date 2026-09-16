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
#include <memory>
#include <string>
#include <vector>

#include "bedrock/bedrock.h"
#include "bedrock/core/threading/task_group.h"
#include "bedrock/deps/nethernet/context.h"
#include "bedrock/deps/webrtc/sigslot.h"

namespace endstone::core {
class EndstoneServer;
}

namespace webrtc {
class Socket;
}

namespace NetherNet {

class HttpConnection;

class HttpServer : public ContextProxy, public sigslot::has_slots<sigslot::single_threaded> {
public:
    ~HttpServer() override;

private:
    std::uint16_t port_;
    std::string bind_address_;
    std::unique_ptr<webrtc::Socket> listen_socket_;
    std::vector<std::shared_ptr<HttpConnection>> connections_;
    TaskGroup task_group_;

    friend class endstone::core::EndstoneServer;
};
BEDROCK_STATIC_ASSERT_SIZE(HttpServer, 504, 416);

}  // namespace NetherNet
