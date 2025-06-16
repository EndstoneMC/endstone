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

#include <string>

#include "bedrock/certificates/certificate.h"
#include "bedrock/certificates/identity/player_authentication_info.h"
#include "bedrock/platform/uuid.h"

class LegacyMultiplayerToken {
public:
    enum class VerificationOptions : int {
        Default = 0,
        IgnoreTimestamp = 1,
    };

    [[nodiscard]] mce::UUID getIdentity() const;
    [[nodiscard]] std::string getIdentityName() const;
    [[nodiscard]] std::string getXuid(bool) const;
    [[nodiscard]] std::string getSignerPublicKey() const;
    [[nodiscard]] time_t getNotBeforeDate() const;
    [[nodiscard]] time_t getExpirationDate() const;
    [[nodiscard]] std::string getIdentityPublicKey() const;
    [[nodiscard]] bool isValid() const
    {
        return certificate && certificate->isValid();
    }
    [[nodiscard]] operator bool() const
    {
        return isValid();
    }
    [[nodiscard]] bool isLoggedIntoXboxLive() const;
    void invalidate();
    [[nodiscard]] PlayerAuthenticationInfo getTrustedInfo(bool) const;

protected:
    LegacyMultiplayerToken(std::unique_ptr<Certificate>, bool);
    std::unique_ptr<Certificate> certificate;
};
static_assert(sizeof(LegacyMultiplayerToken) == 8);
