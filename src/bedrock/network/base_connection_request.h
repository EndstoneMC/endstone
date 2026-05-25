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
#include <vector>

#include "bedrock/certificates/identity/game_server_token.h"
#include "bedrock/certificates/identity/player_authentication_info.h"
#include "bedrock/certificates/web_token.h"
#include "bedrock/common_types.h"
#include "bedrock/core/math/color.h"
#include "bedrock/deps/json/value.h"
#include "bedrock/input/input_mode.h"
#include "bedrock/platform/build_platform.h"
#include "bedrock/resources/min_engine_version.h"

class BaseConnectionRequest {
public:
    virtual ~BaseConnectionRequest() = default;

    [[nodiscard]] PlayerAuthenticationType getAuthenticationType() const;
    [[nodiscard]] std::string getSelfSignedId() const;
    [[nodiscard]] std::uint64_t getClientRandomId() const;
    [[nodiscard]] std::string getPlatformId() const;
    [[nodiscard]] std::string getThirdPartyName() const;
    [[nodiscard]] std::string getPlatformOnlineId() const;
    [[nodiscard]] std::string getPlatformOfflineId() const;
    [[nodiscard]] BuildPlatform getDeviceOS() const;
    [[nodiscard]] std::string getDeviceId() const;
    [[nodiscard]] InputMode getCurrentInputMode() const;
    [[nodiscard]] PlatformType getPlatformType() const;
    [[nodiscard]] std::string getLanguageCode() const;
    [[nodiscard]] std::string getSkinId() const;
    [[nodiscard]] std::string getCapeId() const;
    [[nodiscard]] std::vector<unsigned char> getSkinData() const;
    [[nodiscard]] std::uint16_t getSkinImageWidth() const;
    [[nodiscard]] std::uint16_t getSkinImageHeight() const;
    [[nodiscard]] std::vector<unsigned char> getCapeData() const;
    [[nodiscard]] std::uint16_t getCapeImageWidth() const;
    [[nodiscard]] std::uint16_t getCapeImageHeight() const;
    // std::vector<AnimatedImageData> getAnimatedImageData() const;
    // std::vector<SerializedPersonaPieceHandle> getPersonaPieces() const;
    // std::unordered_map<persona::PieceType, TintMapColor> getPieceTintColors() const;
    [[nodiscard]] std::string getArmSize() const;
    [[nodiscard]] Color getSkinColor() const;
    [[nodiscard]] MinEngineVersion getSkinGeometryMinEngineVersion() const;
    [[nodiscard]] std::string getSkinResourcePatch() const;
    [[nodiscard]] std::string getSkinGeometry() const;
    [[nodiscard]] std::string getSkinAnimationData() const;
    [[nodiscard]] bool isPremiumSkin() const;
    [[nodiscard]] bool isPersonaSkin() const;
    [[nodiscard]] bool isTrustedSkin() const;
    [[nodiscard]] bool isOverrideSkin() const;
    [[nodiscard]] bool isCapeOnClassicSkin() const;
    [[nodiscard]] bool isCompatibleWithClientSideChunkGen() const;
    [[nodiscard]] SyncedClientOptionsComponent getClientOptions() const;
    [[nodiscard]] const RawGameServerToken &_getGameServerToken() const;
    [[nodiscard]] const WebToken &_getRawRequest() const;
    [[nodiscard]] bool isWellFormed() const;

protected:
    virtual bool _isAuthTypeWellFormed() const = 0;
    [[nodiscard]] Json::Value getData(const std::string &key) const;

    std::unique_ptr<WebToken> raw_token_;
    RawGameServerToken unverified_game_server_token_;
    PlayerAuthenticationType authentication_type_;
};
