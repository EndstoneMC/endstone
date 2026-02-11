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

#include "bedrock/certificates/certificate.h"

#include <nlohmann/json.hpp>

#include "endstone/runtime/hook.h"

UnverifiedCertificate UnverifiedCertificate::fromString(const std::string &input)
{
    if (input.size() <= 0x300000) {
        try {
            auto json = nlohmann::json::parse(input);
            if (json.is_object()) {
                auto &chain = json["chain"];
                if (chain.is_array() && chain.size() <= 10) {
                    return ENDSTONE_HOOK_CALL_ORIGINAL(&UnverifiedCertificate::fromString, input);
                }
            }
        }
        catch (...) {
        }
    }
    return {WebToken(), nullptr};
}

bool Certificate::validate(std::time_t current_time, bool is_self_signed, bool check_expired)
{
    // Fix: #blamemojang
    // ServerConnectionAuthValidatorAnon::ConnectionRequest_verify does not check
    // the result of UnverifiedCertificate::verify, this will crash the server if the client
    // sends an invalid certificate.
    if (std::launder(this) == nullptr) {
        return false;
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&Certificate::validate, this, current_time, is_self_signed, check_expired);
}
