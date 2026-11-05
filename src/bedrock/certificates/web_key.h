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
#include <utility>

#include "bedrock/bedrock.h"
#include "bedrock/crypto/asymmetric/asymmetric.h"

class WebKey {
public:
    static WebKey fromRawPublicKey(std::string public_key, Crypto::Asymmetric::System type)
    {
        return {type, std::move(public_key)};
    }
    [[nodiscard]] const std::string &getPublicKey() const;
    [[nodiscard]] Crypto::Asymmetric::System getType() const;

private:
    WebKey(Crypto::Asymmetric::System type, std::string public_key) : type_(type), public_key_(std::move(public_key)) {}

    Crypto::Asymmetric::System type_;
    std::string public_key_;
};
BEDROCK_STATIC_ASSERT_SIZE(WebKey, 40, 32);
