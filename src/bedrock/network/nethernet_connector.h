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

#include <functional>
#include <memory>

#include "bedrock/deps/json/value.h"
#include "bedrock/deps/nethernet/nethernet_transport.h"
#include "bedrock/deps/nethernet/network_id.h"
#include "bedrock/network/disconnection_request_info.h"
#include "bedrock/network/network_identifier.h"
#include "bedrock/network/network_peer.h"
#include "bedrock/network/remote_connector.h"

namespace endstone::core {
class EndstoneServer;
}

namespace Bedrock::Http {
class LibHttpClientInstance;
}

struct NetherNetConnector : RemoteConnector, NetherNet::INetherNetTransportInterfaceCallbacks {
    ~NetherNetConnector() override;

private:
    const std::shared_ptr<Bedrock::Http::LibHttpClientInstance> http_library_;
    const NetherNet::NetworkID network_id_;
    std::unique_ptr<NetherNet::INetherNetTransportInterface,
                    std::function<void(NetherNet::INetherNetTransportInterface *)>>
        transport_;

    friend class endstone::core::EndstoneServer;
};
