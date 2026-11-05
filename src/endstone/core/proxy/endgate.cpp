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

#include "endstone/core/proxy/endgate.h"

#include <algorithm>
#include <string_view>

#include "endstone/core/base64.h"
#include "endstone/core/server.h"

namespace endstone::core {

std::optional<nlohmann::json> EndgateProxy::readPlayerInfo(const ::ConnectionRequest &request)
{
    const auto claims = readSegment(request, 1);
    const auto info = claims.find(PlayerInfoChannel);
    if (info == claims.end() || !info->is_object() || !info->contains("version") ||
        !info->at("version").is_number_integer() || !info->contains("address") || !info->at("address").is_string()) {
        return std::nullopt;
    }
    return *info;
}

bool EndgateProxy::checkIntegrity(const ::ConnectionRequest &request)
{
    const auto &key = EndstoneServer::getInstance().getEndgateForwardingKey();
    const auto header = readSegment(request, 0);
    const auto kid = header.find("kid");
    return request.getAuthenticationType() != PlayerAuthenticationType::SelfSigned && !key.empty() &&
           kid != header.end() && kid->is_string() && kid->get<std::string>() == key;
}

std::string EndgateProxy::readAddress(const nlohmann::json &player_info)
{
    return player_info.at("address").get<std::string>();
}

nlohmann::json EndgateProxy::readSegment(const ::ConnectionRequest &request, const std::size_t index)
{
    const std::string_view token = request._getGameServerToken().value;
    std::size_t begin = 0;
    for (std::size_t i = 0; i < index && begin != std::string_view::npos; ++i) {
        begin = token.find('.', begin);
        begin = begin == std::string_view::npos ? begin : begin + 1;
    }
    const auto end = begin == std::string_view::npos ? begin : token.find('.', begin);
    if (end == std::string_view::npos) {
        return {};
    }

    std::string segment(token.substr(begin, end - begin));
    std::ranges::replace(segment, '-', '+');
    std::ranges::replace(segment, '_', '/');
    segment.append((4 - segment.size() % 4) % 4, '=');
    const auto bytes = base64_decode<std::string>(segment);
    if (!bytes) {
        return {};
    }
    auto json = nlohmann::json::parse(*bytes, nullptr, false);
    return json.is_object() ? json : nlohmann::json{};
}

}  // namespace endstone::core
