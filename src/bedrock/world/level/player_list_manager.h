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
#include <string>
#include <unordered_map>

#include "bedrock/bedrock.h"
#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/core/utility/pub_sub/connector.h"
#include "bedrock/core/utility/pub_sub/publisher.h"
#include "bedrock/core/utility/pub_sub/subscription.h"
#include "bedrock/forward.h"
#include "bedrock/network/packet_sender.h"
#include "bedrock/platform/uuid.h"
#include "bedrock/world/actor/player/player_list_entry.h"
#include "bedrock/world/level/gameplay_user_manager_connector.h"

class PlayerListManager {
public:
    PlayerListManager();
    ~PlayerListManager();
    void initializeWithGameplayUserManagerOnServer(IGameplayUserManagerConnector &gameplay_user_manager_connector);
    void setPacketSender(PacketSender &packet_sender);
    void setPlayerLocationReceiver(Bedrock::NonOwnerPointer<PlayerLocationReceiver> player_location_receiver);
    void setPlayerLocationSender(Bedrock::NonOwnerPointer<PlayerLocationSender> player_location_sender);
    [[nodiscard]] const std::unordered_map<mce::UUID, PlayerListEntry> &getPlayerList() const;
    [[nodiscard]] const std::string &getPlayerXUID(const mce::UUID &uuid) const;
    [[nodiscard]] const std::string &getPlayerPlatformOnlineId(const mce::UUID &uuid) const;
    Bedrock::PubSub::Connector<void(const PlayerListEntry &, const std::unordered_map<mce::UUID, PlayerListEntry> &)> &
    getOnPlayerListEntryAddedConnector();
    Bedrock::PubSub::Connector<void(const PlayerListEntry &, const std::unordered_map<mce::UUID, PlayerListEntry> &)> &
    getOnPlayerListEntryRemovedConnector();
    PlayerListEntry *tryGetPlayerEntry(const mce::UUID &uuid);
    PlayerListEntry *tryGetPlayerEntry(ActorUniqueID player_id);
    void addPlayerEntry(const mce::UUID &uuid, PlayerListEntry &&player_list_entry);
    void removeByUUID(const mce::UUID &uuid);
    void clearPlayerList();

private:
    friend class PlayerLocationSender;  // Endstone

    void _onGameplayUserAdded(EntityContext &entity);
    void _onGameplayUserRemoved(EntityContext &entity);
    void _onAnyGameplayUsersRemoved();
    std::unordered_map<mce::UUID, PlayerListEntry> player_list_;
    Bedrock::NonOwnerPointer<PacketSender> packet_sender_;
    Bedrock::NonOwnerPointer<PlayerLocationReceiver> player_location_receiver_;
    Bedrock::NonOwnerPointer<PlayerLocationSender> player_location_sender_;
    Bedrock::PubSub::Publisher<void(const PlayerListEntry &, const std::unordered_map<mce::UUID, PlayerListEntry> &),
                               Bedrock::PubSub::ThreadModel::MultiThreaded,
                               Bedrock::PubSub::ReturnPolicyType::Aggregate>
        on_player_list_entry_added_;
    Bedrock::PubSub::Publisher<void(const PlayerListEntry &, const std::unordered_map<mce::UUID, PlayerListEntry> &),
                               Bedrock::PubSub::ThreadModel::MultiThreaded,
                               Bedrock::PubSub::ReturnPolicyType::Aggregate>
        on_player_list_entry_removed_;
    Bedrock::PubSub::Subscription on_gameplay_user_added_subscription_;
    Bedrock::PubSub::Subscription on_gameplay_user_removed_subscription_;
    Bedrock::PubSub::Subscription on_any_gameplay_users_removed_subscription_;
    std::unique_ptr<PlayerListPacket> pending_player_list_remove_packet_;
};
