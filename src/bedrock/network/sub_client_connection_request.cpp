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

#include "bedrock/network/sub_client_connection_request.h"

#include "bedrock/crypto/encoding/base64.h"

std::string SubClientConnectionRequest::getSkinId() const
{
    return getData("SkinId").asString();
}

std::string SubClientConnectionRequest::getCapeId() const
{
    return getData("CapeId").asString();
}

std::vector<unsigned char> SubClientConnectionRequest::getSkinData() const
{
    auto result = Util::base64_decode(getData("SkinData").asString());
    return {result.begin(), result.end()};
}

uint16_t SubClientConnectionRequest::getSkinImageWidth() const
{
    return getData("SkinImageWidth").asInt();
}

uint16_t SubClientConnectionRequest::getSkinImageHeight() const
{
    return getData("SkinImageHeight").asInt();
}

std::vector<unsigned char> SubClientConnectionRequest::getCapeData() const
{
    auto result = Util::base64_decode(getData("CapeData").asString());
    return {result.begin(), result.end()};
}

uint16_t SubClientConnectionRequest::getCapeImageWidth() const
{
    return getData("CapeImageWidth").asInt();
}

uint16_t SubClientConnectionRequest::getCapeImageHeight() const
{
    return getData("CapeImageHeight").asInt();
}

bool SubClientConnectionRequest::isVerified() const
{
    return is_verified_ && raw_token_ != nullptr;
}

std::string SubClientConnectionRequest::getGameVersionString() const
{
    return getData("GameVersion").asString();
}

BuildPlatform SubClientConnectionRequest::getDeviceOS() const
{
    return static_cast<BuildPlatform>(getData("DeviceOS").asInt());
}

std::string SubClientConnectionRequest::getDeviceId() const
{
    return getData("DeviceId").asString();
}

std::string SubClientConnectionRequest::getLanguageCode() const
{
    return getData("LanguageCode").asString();
}

Json::Value SubClientConnectionRequest::getData(const std::string &key) const
{
    if (isVerified()) {
        return raw_token_->getData().get(key, Json::nullValue);
    }
    return Json::nullValue;
}
