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

#include "bedrock/network/server_network_handler.h"

#include "bedrock/locale/i18n.h"
#include "bedrock/symbol.h"

void ServerNetworkHandler::updateServerAnnouncement()
{
    ENDSTONE_SYMCALL(&ServerNetworkHandler::updateServerAnnouncement, this);
}

ConnectionRequest const &ServerNetworkHandler::Client::getPrimaryRequest() const
{
    return *primary_request_;
}

std::unordered_map<SubClientId, std::unique_ptr<SubClientConnectionRequest>> const &ServerNetworkHandler::Client::
    getSubClientRequests() const
{
    return sub_client_requests_;
}

const Bedrock::NonOwnerPointer<ILevel> &ServerNetworkHandler::getLevel() const
{
    return level_;
}
