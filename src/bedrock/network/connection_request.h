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

#include "bedrock/network/base_connection_request.h"

class ConnectionRequest : public BaseConnectionRequest {
public:
    [[nodiscard]] std::string getServerAddress() const;
    [[nodiscard]] std::string getEduTokenChain() const;
    [[nodiscard]] std::string getEduSessionToken() const;
    [[nodiscard]] std::string getEduJoinerToHostNonce() const;
    // edu::Role getADRole() const;
    [[nodiscard]] bool isEduMode() const;
    [[nodiscard]] bool isEditorMode() const;
    [[nodiscard]] std::string getGameVersionString() const;
    [[nodiscard]] std::string getDeviceModel() const;
    [[nodiscard]] InputMode getDefaultInputMode() const;
    [[nodiscard]] int getGuiScale() const;
    // std::optional<PlayerPartyInfo> getPartyInfo() const;
    // std::optional<Social::Nonce> getNonce() const;

private:
    [[nodiscard]] bool _isAuthTypeWellFormed() const override;
};
