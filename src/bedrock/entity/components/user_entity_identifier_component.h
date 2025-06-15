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

#include <entt/entt.hpp>

#include "bedrock/certificates/identity/player_authentication_info.h"
#include "bedrock/common_types.h"
#include "bedrock/entity/gamerefs_entity/gamerefs_entity.h"
#include "bedrock/network/network_identifier.h"
#include "bedrock/platform/uuid.h"

class UserEntityIdentifierComponent {
public:
    UserEntityIdentifierComponent(const NetworkIdentifier &, SubClientId, mce::UUID, const std::string &,
                                  PlayerAuthenticationType, const PlayerAuthenticationInfo &);

    static UserEntityIdentifierComponent *tryGetFromEntity(EntityContext &);
    static const UserEntityIdentifierComponent *tryGetFromEntity(const EntityContext &);

    [[nodiscard]] bool isPrimaryClient() const;
    [[nodiscard]] bool isLoggedIntoXboxLive() const;
    [[nodiscard]] const NetworkIdentifier &getNetworkId() const
    {
        return network_id_;
    }
    [[nodiscard]] SubClientId getSubClientId() const
    {
        return client_sub_id_;
    }
    [[nodiscard]] std::string getUnverifiedPlayFabId() const;
    [[nodiscard]] mce::UUID getClientUUID() const
    {
        return client_uuid_;
    }
    [[nodiscard]] std::string getIdentityName() const;
    [[nodiscard]] std::string getXuid(bool trust_self_signed) const
    {
        return trusted_player_info_.xuid;
    }

private:
    NetworkIdentifier network_id_;
    SubClientId client_sub_id_;
    std::string playfab_id_unverified_;
    mce::UUID client_uuid_;
    PlayerAuthenticationType authentication_type_;
    PlayerAuthenticationInfo trusted_player_info_;
};
#if defined __clang__ || defined __GNUC__
static_assert(entt::type_hash<UserEntityIdentifierComponent>::value() == 0xB845379);
#elif defined _MSC_VER
static_assert(entt::type_hash<UserEntityIdentifierComponent>::value() == 0x7CF39851);
#endif
