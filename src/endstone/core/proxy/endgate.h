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

#include <cstddef>
#include <optional>
#include <string>

#include <nlohmann/json.hpp>

#include "bedrock/network/connection_request.h"

namespace endstone::core {

/**
 * Endgate's identity forwarding re-signs the player's validated login token with endgate's own key and adds the
 * player's details as a claim, so they ride in the login BDS already verifies and need no message of their own.
 *
 * @see com.destroystokyo.paper.proxy.VelocityProxy
 */
class EndgateProxy {
    /**
     * @see com.destroystokyo.paper.proxy.VelocityProxy#SUPPORTED_FORWARDING_VERSION
     */
    static constexpr int SupportedForwardingVersion = 1;

public:
    /**
     * @see com.destroystokyo.paper.proxy.VelocityProxy#MAX_SUPPORTED_FORWARDING_VERSION
     */
    static constexpr int MaxSupportedForwardingVersion = SupportedForwardingVersion;

    /**
     * @see com.destroystokyo.paper.proxy.VelocityProxy#PLAYER_INFO_CHANNEL
     */
    static constexpr auto PlayerInfoChannel = "endgate:player_info";

    /**
     * Returns the player info endgate added to the login token, or nothing when the login didn't come through endgate.
     *
     * @see net.minecraft.network.protocol.login.ServerboundCustomQueryAnswerPacket#payload
     */
    [[nodiscard]] static std::optional<nlohmann::json> readPlayerInfo(const ::ConnectionRequest &request);

    /**
     * Checks that the login token was signed with endgate's forwarding key, which BDS has already verified the
     * signature against.
     *
     * @see com.destroystokyo.paper.proxy.VelocityProxy#checkIntegrity
     */
    [[nodiscard]] static bool checkIntegrity(const ::ConnectionRequest &request);

    /**
     * @see com.destroystokyo.paper.proxy.VelocityProxy#readAddress
     */
    [[nodiscard]] static std::string readAddress(const nlohmann::json &player_info);

private:
    [[nodiscard]] static nlohmann::json readSegment(const ::ConnectionRequest &request, std::size_t index);
};

}  // namespace endstone::core
