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
#include "bedrock/shared_constants.h"
#include "bedrock/symbol.h"

ConnectionRequest const &ServerNetworkHandler::Client::getPrimaryRequest() const
{
    return *primary_request_;
}

std::unordered_map<SubClientId, std::unique_ptr<SubClientConnectionRequest>> const &ServerNetworkHandler::Client::
    getSubClientRequests() const
{
    return sub_client_requests_;
}

int ServerNetworkHandler::getMaxNumPlayers() const
{
    return max_num_players_;
}

int ServerNetworkHandler::setMaxNumPlayers(int max_players)
{
    auto player_count = _getActiveAndInProgressPlayerCount(mce::UUID::EMPTY);
    auto result = 0;
    if (max_players <= SharedConstants::NetworkDefaultMaxConnections) {
        if (max_players < player_count) {
            max_players = player_count;
            result = -1;
        }
    }
    else {
        max_players = SharedConstants::NetworkDefaultMaxConnections;
        result = 1;
    }

    if (max_num_players_ != max_players) {
        max_num_players_ = max_players;
        updateServerAnnouncement();
        app_.onNetworkMaxPlayersChanged(max_num_players_);
    }
    return result;
}

void ServerNetworkHandler::updateServerAnnouncement()
{
    BEDROCK_CALL(&ServerNetworkHandler::updateServerAnnouncement, this);
}
