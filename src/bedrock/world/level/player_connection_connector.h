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

#include "bedrock/core/utility/pub_sub/connector.h"
#include "bedrock/network/network_identifier.h"
#include "bedrock/world/level/block_pos.h"
#include "bedrock/world/level/dimension/dimension_type.h"

class IPlayerConnectionConnector {
public:
    virtual ~IPlayerConnectionConnector() = default;
    virtual Bedrock::PubSub::Connector<void(const NetworkIdentifierWithSubId &)> &getOnPlayerNetworkDisconnectedConnector() = 0;
    virtual Bedrock::PubSub::Connector<void(const NetworkIdentifierWithSubId &, const BlockPos &, unsigned int, DimensionType)> &
    getOnPlayerNetworkConnectedConnector() = 0;
};
static_assert(sizeof(IPlayerConnectionConnector) == 8);
