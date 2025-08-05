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

#include "bedrock/network/connector.h"

class RemoteConnector : public Connector,
                        public NetworkEnableDisableListener,
                        public Bedrock::EnableNonOwnerReferences {
public:
    RemoteConnector(ConnectionCallbacks &, const Bedrock::NonOwnerPointer<AppPlatform> &);
    virtual bool host(const ConnectionDefinition &) = 0;
    virtual bool connect(const Social::GameConnectionInfo &, const Social::GameConnectionInfo &) = 0;
    virtual void disconnect() = 0;
    virtual void tick() = 0;
    virtual void runEvents() = 0;
    [[nodiscard]] virtual bool isServer() const = 0;
    virtual void closeNetworkConnection(const NetworkIdentifier &) = 0;
    [[nodiscard]] virtual NetworkIdentifier getNetworkIdentifier() const = 0;
    virtual bool setApplicationHandshakeCompleted(const NetworkIdentifier &) = 0;
    virtual void setDisableLanSignaling(bool) = 0;
};
static_assert(sizeof(RemoteConnector) == 80);
