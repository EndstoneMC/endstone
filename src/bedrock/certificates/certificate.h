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

#include "bedrock/bedrock.h"
#include "bedrock/certificates/web_token.h"

class UnverifiedCertificate {
public:
    static UnverifiedCertificate fromString(const std::string &input);
    friend class Certificate;

private:
    UnverifiedCertificate(const WebToken &raw_token,
                          std::unique_ptr<UnverifiedCertificate> parent_unverified_certificate)
        : raw_token_(raw_token), parent_unverified_certificate_(std::move(parent_unverified_certificate))
    {
    }
    WebToken raw_token_;                                                    // +0
    std::unique_ptr<UnverifiedCertificate> parent_unverified_certificate_;  // +128
};

class Certificate {
public:
    [[nodiscard]] Json::Value getExtraData(const std::string &key, const Json::Value &default_value) const;
    [[nodiscard]] bool isValid() const;
    [[nodiscard]] bool isSelfSigned() const;

private:
    UnverifiedCertificate unverified_certificate_;     // +0
    std::unique_ptr<Certificate> parent_certificate_;  // +136
    bool is_valid_;                                    // +144
    bool is_self_signed_;                              // +145
};
