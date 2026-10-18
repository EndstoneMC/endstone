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

#include "bedrock/world/level/dimension/dimension.h"

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <optional>
#include <vector>

#include <gsl/util>

#include "bedrock/entity/components/actor_unique_id_component.h"
#include "bedrock/entity/components/user_entity_identifier_component.h"
#include "bedrock/entity/weak_entity_ref.h"
#include "bedrock/network/packet_sender.h"
#include "bedrock/server/server_player.h"
#include "bedrock/world/level/level.h"
#include "endstone/runtime/hook.h"

namespace {
struct ActiveUser {
    std::optional<EntityContext> entity;
    Player *player = nullptr;
};

struct ActiveUserSnapshot {
    const std::vector<WeakEntityRef> *users = nullptr;
    std::vector<std::byte> bytes;
    std::vector<ActiveUser> entries;
    int depth = 0;
};
ActiveUserSnapshot snapshot;

std::vector<ActiveUser> *snapshotActiveUsers(const std::vector<WeakEntityRef> &users)
{
    const auto *data = reinterpret_cast<const std::byte *>(users.data());
    const auto size = users.size() * sizeof(WeakEntityRef);
    if (snapshot.users == &users && snapshot.bytes.size() == size &&
        (size == 0 || std::memcmp(snapshot.bytes.data(), data, size) == 0)) {
        return &snapshot.entries;
    }
    if (snapshot.depth > 0) {
        return nullptr;
    }
    snapshot.users = &users;
    snapshot.bytes.assign(data, data + size);
    snapshot.entries.clear();
    for (const auto &user : users) {
        auto &entry = snapshot.entries.emplace_back();
        if (auto entity = user.weak_entity.unwrap()) {
            entry.entity.emplace(*entity);
            entry.player = Player::tryGetFromEntity(*entry.entity, true);
        }
    }
    return &snapshot.entries;
}

Player *resolve(ActiveUser &user, bool include_removed)
{
    if (!user.entity || !user.entity->isValid()) {
        return nullptr;
    }
    if (!user.player) {
        return Player::tryGetFromEntity(*user.entity, include_removed);
    }
    return include_removed || !user.player->isRemoved() ? user.player : nullptr;
}
}  // namespace

void Dimension::sendPacketForEntity(const Actor &actor, const Packet &packet, const Player *except)
{
    // #blameMojang - every player in the level is asked whether the actor is relevant, three unique id lookups
    // and a hash each. Fix: read the actor's id once and look it up in each player's nearby actors.
    const auto *actor_id = actor.tryGetComponent<const ActorUniqueIDComponent>();
    if (!actor_id || !actor_id->unique_id.isValid()) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&Dimension::sendPacketForEntity, this, actor, packet, except);
        return;
    }
    const auto id = actor_id->unique_id;
    std::vector<NetworkIdentifierWithSubId> recipients;
    auto fallback = false;
    forEachPlayer([&](Player &player) {
        if (&player == except) {
            return true;
        }
        const auto *player_id = player.tryGetComponent<ActorUniqueIDComponent>();
        if (!player_id || !player_id->unique_id.isValid()) {
            fallback = true;
            return false;
        }
        if (player_id->unique_id != id && !static_cast<ServerPlayer &>(player).getNearbyActors().contains(id)) {
            return true;
        }
        if (const auto *user = player.tryGetComponent<UserEntityIdentifierComponent>()) {
            recipients.push_back({user->getNetworkId(), user->getSubClientId()});
        }
        return true;
    });
    if (fallback) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&Dimension::sendPacketForEntity, this, actor, packet, except);
        return;
    }
    getLevel().getPacketSender()->sendToClients(recipients, packet);
}

void Dimension::forEachPlayer(brstd::function_ref<bool(Player &)> callback) const
{
    // #blameMojang - each call locks every active user's weak ref and looks up three components, thousands of
    // times per tick. Fix: walk a snapshot of the level's active users, rebuilt when the list changes.
    auto *users = snapshotActiveUsers(getLevel().getActiveUsers());
    if (!users) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&Dimension::forEachPlayer, this, callback);
        return;
    }
    const auto id = getDimensionId();
    ++snapshot.depth;
    const auto leave = gsl::finally([] { --snapshot.depth; });
    for (auto &user : *users) {
        if (auto *player = resolve(user, false);
            player != nullptr && player->getDimensionId() == id && !callback(*player)) {
            break;
        }
    }
}

int Level::getActivePlayerCount() const
{
    auto *users = snapshotActiveUsers(getActiveUsers());
    if (!users) {
        return ENDSTONE_HOOK_CALL_ORIGINAL(&Level::getActivePlayerCount, this);
    }
    return static_cast<int>(std::ranges::count_if(*users, [](auto &user) { return resolve(user, true) != nullptr; }));
}
