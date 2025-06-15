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
#include "bedrock/certificates/web_token.h"
#include "bedrock/deps/json/value.h"

class SubClientConnectionRequest {
public:
private:
    [[nodiscard]] Json::Value getData(const std::string &key) const;

    std::unique_ptr<WebToken> raw_token_;
    std::unique_ptr<UnverifiedCertificate> certificate_data_;
    LegacyMultiplayerToken legacy_multiplayer_token_;
    RawGameServerToken game_server_token_;
    PlayerAuthenticationType authentication_type_;
};
