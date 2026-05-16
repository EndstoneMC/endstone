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

#include "bedrock/network/base_connection_request.h"

#include "bedrock/crypto/encoding/base64.h"

Json::Value BaseConnectionRequest::getData(const std::string &key) const
{
    return raw_token_->getData().get(key, Json::nullValue);
}

const WebToken &BaseConnectionRequest::_getRawRequest() const
{
    return *raw_token_;
}

std::string BaseConnectionRequest::getThirdPartyName() const
{
    return getData("ThirdPartyName").asString();
}

std::string BaseConnectionRequest::getSkinId() const
{
    return getData("SkinId").asString();
}

std::string BaseConnectionRequest::getCapeId() const
{
    return getData("CapeId").asString();
}

std::vector<unsigned char> BaseConnectionRequest::getSkinData() const
{
    auto result = Util::base64_decode(getData("SkinData").asString());
    return {result.begin(), result.end()};
}

uint16_t BaseConnectionRequest::getSkinImageWidth() const
{
    return getData("SkinImageWidth").asInt();
}

uint16_t BaseConnectionRequest::getSkinImageHeight() const
{
    return getData("SkinImageHeight").asInt();
}

std::vector<unsigned char> BaseConnectionRequest::getCapeData() const
{
    auto result = Util::base64_decode(getData("CapeData").asString());
    return {result.begin(), result.end()};
}

uint16_t BaseConnectionRequest::getCapeImageWidth() const
{
    return getData("CapeImageWidth").asInt();
}

uint16_t BaseConnectionRequest::getCapeImageHeight() const
{
    return getData("CapeImageHeight").asInt();
}

BuildPlatform BaseConnectionRequest::getDeviceOS() const
{
    return static_cast<BuildPlatform>(getData("DeviceOS").asInt());
}

std::string BaseConnectionRequest::getDeviceId() const
{
    return getData("DeviceId").asString();
}

std::string BaseConnectionRequest::getLanguageCode() const
{
    return getData("LanguageCode").asString();
}
