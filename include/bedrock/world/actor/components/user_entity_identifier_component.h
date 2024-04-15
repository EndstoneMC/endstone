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

#include "bedrock/mce.h"
#include "bedrock/network/network_identifier.h"
#include "bedrock/network/protocol/certificate.h"
#include "bedrock/network/protocol/sub_client_id.h"

class UserEntityIdentifierComponent {
public:
    NetworkIdentifier network_id;              // +0
    SubClientId sub_client_id;                 // +160
    mce::UUID uuid;                            // +168
    std::string pfid;                          // +184
    std::unique_ptr<Certificate> certificate;  // +216
};
