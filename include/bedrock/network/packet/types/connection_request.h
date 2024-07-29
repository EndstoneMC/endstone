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
#include "bedrock/certificates/unverified_certificate.h"
#include "bedrock/certificates/web_token.h"
#include "bedrock/deps/jsoncpp/value.h"
#include "bedrock/network/sub_client_id.h"

class ConnectionRequest {
public:
    [[nodiscard]] Json::Value getData(const std::string &key) const
    {
        if (certificate_ && raw_token_) {
            return raw_token_->data_info.get(key, Json::nullValue);
        }
        return Json::nullValue;
    }

private:
    std::unique_ptr<UnverifiedCertificate> certificate_data_;  // +0 std::unique_ptr<UnverifiedCertificate>
    std::unique_ptr<Certificate> certificate_;                 // +8 std::unique_ptr<Certificate>
    std::unique_ptr<WebToken> raw_token_;                      // +16
    SubClientId sub_client_id_;                                // +24
};
