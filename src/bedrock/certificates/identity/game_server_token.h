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

#include "bedrock/certificates/certificate.h"

class GameServerToken {
public:
    enum class VerificationOptions : int {
        Default = 0,
        IgnoreTimestamp = 1,
    };

    GameServerToken();
    GameServerToken(std::unique_ptr<Certificate>, VerificationOptions);

    [[nodiscard]] std::string getXuid(bool trust_self_signed) const
    {
        if (!isValid() || (!trust_self_signed && certificate_->isSelfSigned())) {
            return "";
        }
        return certificate_->getExtraData("XUID", {}).asString();
    }

    operator bool() const
    {
        return isValid();
    }

    [[nodiscard]] bool isValid() const
    {
        return certificate_ && certificate_->isValid();
    }

protected:
    GameServerToken(std::unique_ptr<Certificate>, bool);
    std::unique_ptr<Certificate> certificate_;  // +0
};
