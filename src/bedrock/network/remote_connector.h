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

#include <memory>

#include "bedrock/bedrock.h"
#include "bedrock/core/utility/enable_non_owner_references.h"
#include "bedrock/forward.h"
#include "bedrock/network/connection_definition.h"
#include "bedrock/network/connector.h"
#include "bedrock/network/network_enable_disable_listener.h"

class RemoteConnector : public Connector,
                        public NetworkEnableDisableListener,
                        public Bedrock::EnableNonOwnerReferences {
public:
    RemoteConnector(ConnectionCallbacks &, IsNetworkAllowed);
    virtual bool host(const ConnectionDefinition &, const PrivateKeyManager &) = 0;
    virtual bool connect(const Social::GameConnectionInfo &, const Social::GameConnectionInfo &,
                         std::shared_ptr<NetherNet::IIdentityAssertionGenerator>) = 0;
    virtual void disconnect() = 0;
    virtual void tick() = 0;
    virtual void runEvents() = 0;
    [[nodiscard]] virtual bool isServer() const = 0;
    virtual void closeNetworkConnection(const NetworkIdentifier &) = 0;
    [[nodiscard]] virtual NetworkIdentifier getNetworkIdentifier() const = 0;
    virtual bool setApplicationHandshakeCompleted(const NetworkIdentifier &) = 0;
    virtual void setDisableLanSignaling(bool) = 0;
};
BEDROCK_STATIC_ASSERT_SIZE(RemoteConnector, 120, 128);
