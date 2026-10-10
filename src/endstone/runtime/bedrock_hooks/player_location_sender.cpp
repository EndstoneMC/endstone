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

#include "bedrock/world/level/player_location_sender.h"

#include <algorithm>
#include <cstdint>
#include <optional>
#include <string_view>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>

#include "bedrock/core/utility/pub_sub/subscription.h"
#include "bedrock/entity/components/player_component.h"
#include "bedrock/world/actor/player/player.h"
#include "bedrock/world/actor/player/player_list_entry.h"
#include "bedrock/world/level/level.h"
#include "bedrock/world/level/player_list_manager.h"
#include "endstone/core/level/level.h"
#include "endstone/core/server.h"
#include "endstone/runtime/hook.h"

namespace {
enum class PairAction : std::uint8_t {
    Other,
    CheckPairs,
    HideAll,
};

PairAction classify(const std::type_info &type)
{
#ifdef _WIN32
    const std::string_view name = type.raw_name();
#else
    const std::string_view name = type.name();
#endif
    if (name.find("checkPlayerPairsAndMaybeSendPackets") != std::string_view::npos) {
        return PairAction::CheckPairs;
    }
    if (name.find("sendPacketsHidingAllPlayers") != std::string_view::npos) {
        return PairAction::HideAll;
    }
    return PairAction::Other;
}

enum class SentState : std::uint8_t {
    Absent,
    Nullopt,
    Position,
};

struct SentPosition {
    Vec3 position{};
    SentState state = SentState::Absent;
};

class SentPositions {
public:
    void clear()
    {
        slots_.clear();
        free_slots_.clear();
        entries_.clear();
        capacity_ = 0;
        used_ = 0;
    }

    std::uint32_t slot(ActorUniqueID id)
    {
        if (const auto it = slots_.find(id.raw_id); it != slots_.end()) {
            return it->second;
        }
        std::uint32_t slot;
        if (!free_slots_.empty()) {
            slot = free_slots_.back();
            free_slots_.pop_back();
        }
        else {
            slot = used_++;
            if (slot >= capacity_) {
                grow(capacity_ == 0 ? 128 : capacity_ * 2);
            }
        }
        slots_.emplace(id.raw_id, slot);
        return slot;
    }

    SentPosition &at(std::uint32_t viewer, std::uint32_t observed)
    {
        return entries_[static_cast<std::size_t>(viewer) * capacity_ + observed];
    }

    void remove(ActorUniqueID id)
    {
        const auto it = slots_.find(id.raw_id);
        if (it == slots_.end()) {
            return;
        }
        const auto slot = it->second;
        for (std::uint32_t i = 0; i < capacity_; ++i) {
            at(slot, i) = {};
            at(i, slot) = {};
        }
        slots_.erase(it);
        free_slots_.push_back(slot);
    }

private:
    void grow(std::uint32_t capacity)
    {
        std::vector<SentPosition> entries(static_cast<std::size_t>(capacity) * capacity);
        for (std::uint32_t i = 0; i < capacity_; ++i) {
            std::copy_n(entries_.begin() + static_cast<std::ptrdiff_t>(i) * capacity_, capacity_,
                        entries.begin() + static_cast<std::ptrdiff_t>(i) * capacity);
        }
        entries_ = std::move(entries);
        capacity_ = capacity;
    }

    std::unordered_map<std::int64_t, std::uint32_t> slots_;
    std::vector<std::uint32_t> free_slots_;
    std::vector<SentPosition> entries_;
    std::uint32_t capacity_ = 0;
    std::uint32_t used_ = 0;
};

const PlayerLocationSender *sent_positions_owner = nullptr;
SentPositions sent_positions;
std::optional<Bedrock::PubSub::Subscription> on_gameplay_user_removed;
}  // namespace

void PlayerLocationSender::_forEachClientPlayerPair(
    const std::vector<WeakEntityRef> &gameplay_users,
    std::function<void(const UserEntityIdentifierComponent &, const Player &, const Player &)> action)
{
    // #blameMojang - vanilla looks each pair up in a flat map ordered by a hash it recomputes on every probe.
    const auto kind = action ? classify(action.target_type()) : PairAction::Other;
    if (kind == PairAction::Other) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&PlayerLocationSender::_forEachClientPlayerPair, this, gameplay_users,
                                    std::move(action));
        return;
    }

    struct ObservedPlayer {
        ActorUniqueID id;
        std::uint32_t slot;
        const PlayerLocationData *data;
    };
    PlayerLocationSender *sender = nullptr;
    const Player *last_viewer = nullptr;
    ActorUniqueID viewer_id;
    std::uint32_t viewer_slot = 0;
    DimensionType viewer_dimension{};
    std::unordered_map<const Player *, ObservedPlayer> observed_players;
    decltype(sent_player_data_)::containers window;
    window.keys.reserve(2);
    window.values.reserve(2);

    // The caller does not pass `this`; the sender is reached through the player list manager.
    const auto bind_sender = [&](const Player &viewer) {
        if (sender) {
            return;
        }
        sender = viewer.getLevel().getPlayerListManager()->player_location_sender_.access();
        if (sender != sent_positions_owner) {
            sent_positions.clear();
            sent_positions_owner = sender;
        }
        if (on_gameplay_user_removed) {
            return;
        }
        // Runs before PlayerListManager's handler, which drops the player's pairs and its player list entry.
        auto &level =
            static_cast<endstone::core::EndstoneLevel *>(endstone::core::EndstoneServer::getInstance().getLevel())
                ->getHandle();
        on_gameplay_user_removed = level.getGameplayUserManager()->getGameplayUserRemovedConnector().connect(
            [&level](EntityContext &entity) {
                if (!sent_positions_owner || !entity.hasComponent<PlayerComponent>()) {
                    return;
                }
                const auto *user = entity.tryGetComponent<UserEntityIdentifierComponent>();
                const auto &location_sender = level.getPlayerListManager()->player_location_sender_;
                if (!user || !location_sender || location_sender.access() != sent_positions_owner) {
                    return;
                }
                for (const auto &[uuid, entry] : level.getPlayerList()) {
                    if (uuid == user->getClientUUID()) {
                        sent_positions.remove(entry.id);
                        break;
                    }
                }
            },
            Bedrock::PubSub::ConnectPosition::AtFront, nullptr);
    };

    const auto run_action = [&](const UserEntityIdentifierComponent &user, const Player &viewer, const Player &observed,
                                const SentPosition *entry) {
        window.keys.clear();
        window.values.clear();
        if (entry && entry->state == SentState::Position) {
            window.keys.emplace_back(viewer_id, observed.getOrCreateUniqueID());
            window.values.emplace_back(entry->position);
        }
        auto real = std::move(sender->sent_player_data_).extract();
        sender->sent_player_data_.replace(std::move(window.keys), std::move(window.values));
        action(user, viewer, observed);
        window = std::move(sender->sent_player_data_).extract();
        sender->sent_player_data_.replace(std::move(real.keys), std::move(real.values));
    };

    const auto should_send = [&](const Player &viewer, const OptionalPosition &prev, const PlayerLocationData &data) {
#ifdef _WIN32
        return sender->_shouldSendPositionPacket(viewer.getPosition(), viewer_dimension, false, prev, data);
#elif __linux__
        return _shouldSendPositionPacket(viewer.getPosition(), viewer_dimension, false, prev, data,
                                         sender->simulation_distance_);
#endif
    };

    const auto hide_pair = [&](const UserEntityIdentifierComponent &user, const Player &viewer,
                               const Player &observed) {
        bind_sender(viewer);
        run_action(user, viewer, observed, nullptr);
    };

    const auto check_pair = [&](const UserEntityIdentifierComponent &user, const Player &viewer,
                                const Player &observed) {
        bind_sender(viewer);
        if (&viewer != last_viewer) {
            last_viewer = &viewer;
            viewer_id = viewer.getOrCreateUniqueID();
            viewer_slot = sent_positions.slot(viewer_id);
            viewer_dimension = viewer.getDimensionId();
        }

        auto [it, inserted] = observed_players.try_emplace(&observed);
        auto &observed_player = it->second;
        if (inserted) {
            observed_player.id = observed.getOrCreateUniqueID();
            observed_player.slot = sent_positions.slot(observed_player.id);
            const auto &keys = sender->current_player_location_data_.keys();
            const auto key = std::ranges::lower_bound(keys, observed_player.id.raw_id, {}, &ActorUniqueID::raw_id);
            observed_player.data = key != keys.end() && *key == observed_player.id
                                     ? &sender->current_player_location_data_.values()[key - keys.begin()]
                                     : nullptr;
        }

        auto &entry = sent_positions.at(viewer_slot, observed_player.slot);
        const auto *data = observed_player.data;
        if (data && entry.state != SentState::Absent && !data->is_spectator) {
            if (entry.state == SentState::Nullopt) {
                if (!should_send(viewer, std::nullopt, *data)) {
                    return;
                }
            }
            else if (data->position && data->dimension == viewer_dimension &&
                     !should_send(viewer, entry.position, *data)) {
                return;
            }
        }

        run_action(user, viewer, observed, &entry);
        const auto pair = std::pair{viewer_id, observed_player.id};
        entry = {};
        for (std::size_t i = 0; i < window.keys.size(); ++i) {
            if (window.keys[i] != pair) {
                continue;
            }
            if (const auto &position = window.values[i]) {
                entry = {*position, SentState::Position};
            }
            else {
                entry.state = SentState::Nullopt;
            }
        }
    };

    if (kind == PairAction::HideAll) {
        ENDSTONE_HOOK_CALL_ORIGINAL(&PlayerLocationSender::_forEachClientPlayerPair, this, gameplay_users, hide_pair);
        sent_positions.clear();
        sent_positions_owner = nullptr;
        return;
    }
    ENDSTONE_HOOK_CALL_ORIGINAL(&PlayerLocationSender::_forEachClientPlayerPair, this, gameplay_users, check_pair);
}
