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

#include <entt/entt.hpp>

#include "bedrock/certificates/certificate.h"
#include "bedrock/common_types.h"
#include "bedrock/network/network_identifier.h"
#include "bedrock/platform/uuid.h"

class UserEntityIdentifierComponent {
public:
    NetworkIdentifier network_id;              // +0
    SubClientId client_sub_id;                 // +160
    mce::UUID client_uuid;                     // +168
    std::string playfab_id_unverified;         // +184
    std::unique_ptr<Certificate> certificate;  // +216
};
#if defined __clang__ || defined __GNUC__
static_assert(entt::type_hash<UserEntityIdentifierComponent>::value() == 0xB845379);
#elif defined _MSC_VER
static_assert(entt::type_hash<UserEntityIdentifierComponent>::value() == 0x7CF39851);
#endif
