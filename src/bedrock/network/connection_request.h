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

#include "bedrock/certificates/certificate.h"
#include "bedrock/certificates/identity/game_server_token.h"
#include "bedrock/certificates/identity/legacy_multiplayer_token.h"
#include "bedrock/certificates/identity/player_authentication_info.h"
#include "bedrock/certificates/web_token.h"
#include "bedrock/common_types.h"
#include "bedrock/core/math/color.h"
#include "bedrock/deps/json/value.h"
#include "bedrock/input/input_mode.h"
#include "bedrock/platform/build_platform.h"
#include "bedrock/resources/min_engine_version.h"

class ConnectionRequest {
public:
    [[nodiscard]] const PlayerAuthenticationInfo &getAuthenticationInfo() const;
    [[nodiscard]] PlayerAuthenticationType getAuthenticationType() const;
    [[nodiscard]] std::string getSelfSignedId() const;
    [[nodiscard]] std::string getServerAddress() const;
    [[nodiscard]] uint64_t getClientRandomId() const;
    [[nodiscard]] std::string getClientPlatformId() const;
    [[nodiscard]] std::string getClientThirdPartyName() const;
    [[nodiscard]] std::string getClientPlatformOnlineId() const;
    [[nodiscard]] std::string getClientPlatformOfflineId() const;
    [[nodiscard]] std::string getSkinId() const;
    [[nodiscard]] std::string getPlayFabIdUnverified() const;
    [[nodiscard]] std::string getCapeId() const;
    [[nodiscard]] std::vector<unsigned char> getSkinData() const;
    [[nodiscard]] std::string getSkinDataAsString() const;
    [[nodiscard]] uint16_t getSkinImageWidth() const;
    [[nodiscard]] uint16_t getSkinImageHeight() const;
    [[nodiscard]] std::vector<unsigned char> getCapeData() const;
    [[nodiscard]] uint16_t getCapeImageWidth() const;
    [[nodiscard]] uint16_t getCapeImageHeight() const;
    // std::vector<AnimatedImageData> getAnimatedImageData() const;
    // std::vector<SerializedPersonaPieceHandle> getPersonaPieces() const;
    // std::unordered_map<persona::PieceType, TintMapColor> getPieceTintColors() const;
    [[nodiscard]] std::string getArmSize() const;
    [[nodiscard]] Color getSkinColor() const;
    [[nodiscard]] std::string getSkinResourcePatch() const;
    [[nodiscard]] std::string getSkinGeometry() const;
    [[nodiscard]] MinEngineVersion getSkinGeometryMinEngineVersion() const;
    [[nodiscard]] std::string getSkinAnimationData() const;
    [[nodiscard]] std::string getEduTokenChain() const;
    // edu::Role getADRole() const;
    [[nodiscard]] bool isEduMode() const;
    [[nodiscard]] bool isEditorMode() const;
    [[nodiscard]] std::string getGameVersionString() const;
    [[nodiscard]] std::string getDeviceModel() const;
    [[nodiscard]] BuildPlatform getDeviceOS() const;
    [[nodiscard]] InputMode getDefaultInputMode() const;
    [[nodiscard]] InputMode getCurrentInputMode() const;
    [[nodiscard]] int getGuiScale() const;
    [[nodiscard]] std::string getDeviceId() const;
    [[nodiscard]] bool isPremiumSkin() const;
    [[nodiscard]] bool isPersonaSkin() const;
    [[nodiscard]] bool isTrustedSkin() const;
    [[nodiscard]] bool isOverrideSkin() const;
    [[nodiscard]] bool isCapeOnClassicSkin() const;
    [[nodiscard]] bool isVerified() const;
    bool verify(const std::vector<std::string> &, int64_t, bool);
    bool verifySelfSigned(bool);
    [[nodiscard]] bool isCompatibleWithClientSideChunkGen() const;
    [[nodiscard]] PlatformType getPlatformType() const;
    [[nodiscard]] SyncedClientOptionsComponent getClientOptions() const;
    [[nodiscard]] bool isValid() const;

    [[nodiscard]] std::string getLanguageCode() const;  // Endstone

private:
    [[nodiscard]] Json::Value getData(const std::string &key) const;

    bool is_verified_;
    std::unique_ptr<WebToken> raw_token_;
    std::unique_ptr<UnverifiedCertificate> certificate_data_;
    LegacyMultiplayerToken legacy_multiplayer_token_;
    RawGameServerToken unverified_game_server_token_;
    GameServerToken verified_game_server_token_;
    PlayerAuthenticationType authentication_type_;
    PlayerAuthenticationInfo authentication_info_;
};
