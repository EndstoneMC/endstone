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
#include <functional>
#include <map>
#include <memory>
#include <unordered_map>
#include <vector>

#include <gsl/pointers>

#include "bedrock/core/utility/pub_sub/connector.h"
#include "bedrock/core/utility/pub_sub/publisher.h"
#include "bedrock/world/actor/actor_unique_id.h"
#include "bedrock/world/server_waypoint.h"
#include "bedrock/world/waypoint_group.h"

class ServerWaypointGroup : public WaypointGroup {
    enum class Action : std::uint8_t {
        None = 0,
        Add = 1,
        Remove = 2,
        Update = 3,
    };

public:
    struct WaypointChangeRecord {
        Action action;
        std::uint32_t update_flags;
    };
    WaypointHandle add(std::unique_ptr<ServerWaypoint> waypoint);
    ServerWaypoint *get(const WaypointHandle &handle);
    [[nodiscard]] bool has(const WaypointHandle &handle) const override;
    bool remove(const WaypointHandle &handle) override;
    [[nodiscard]] std::vector<WaypointHandle> getAllHandlesWithActorID(const ActorUniqueID &id) const;
    std::map<WaypointHandle, WaypointChangeRecord> consumeChanges();
    void forEach(std::function<void(const WaypointHandle &, gsl::not_null<const ServerWaypoint *>)> callback) const;
    void update(const Player &viewing_player);
    Bedrock::PubSub::Connector<void(const std::vector<WaypointHandle> &)> &getOnInvalidActorRemovedEvent();

private:
    friend class VanillaWaypointManager;  // Endstone

    std::unordered_map<WaypointHandle, std::unique_ptr<ServerWaypoint>, WaypointHandle::Hasher> waypoints_;
    std::map<WaypointHandle, WaypointChangeRecord> change_records_;
    Bedrock::PubSub::Publisher<void(const std::vector<WaypointHandle> &), Bedrock::PubSub::ThreadModel::MultiThreaded,
                               Bedrock::PubSub::ReturnPolicyType::Aggregate>
        on_invalid_actor_removed_event_;
};
