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

// #blameMojang - DoS via malformed LoginPacket. Zero input validation on certificate chains:
// 1. Oversized strings (>3MB) cause memory exhaustion
// 2. Large chain arrays cause stack overflow - because someone thought recursion was a good idea
//    for parsing untrusted input with no depth limit
// Fix: reject oversized inputs and excessive chain lengths before processing.
UnverifiedCertificate UnverifiedCertificate::fromString(const std::string &input)
{
    // Reject oversized inputs (>3MB is absurd for a certificate chain)
    if (input.size() <= 0x300000) {
        try {
            auto json = nlohmann::json::parse(input);
            if (json.is_object()) {
                auto &chain = json["chain"];
                // Reject chains with more than 10 certificates (normal auth needs 2-3)
                if (chain.is_array() && chain.size() <= 10) {
                    return ENDSTONE_HOOK_CALL_ORIGINAL(&UnverifiedCertificate::fromString, input);
                }
            }
        }
        catch (...) {
            // Malformed JSON - reject silently
        }
    }
    return {WebToken(), nullptr};
}

bool Certificate::validate(std::time_t current_time, bool is_self_signed, bool check_expired)
{
    // #blameMojang - ConnectionRequest_verify calls validate() without null-checking the result of
    // UnverifiedCertificate::verify(). Malformed certificates -> nullptr -> instant crash.
    // A single null check would have prevented this. Fix: we add the null check ourselves.
    if (std::launder(this) == nullptr) {
        return false;
    }
    return ENDSTONE_HOOK_CALL_ORIGINAL(&Certificate::validate, this, current_time, is_self_signed, check_expired);
}
