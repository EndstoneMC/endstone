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

#include "bedrock/core/utility/enable_non_owner_references.h"

class IPacketObserver : public Bedrock::EnableNonOwnerReferences {
public:
    virtual void packetSentTo(const NetworkIdentifier &, const Packet &, uint32_t) = 0;
    virtual void packetReceivedFrom(const NetworkIdentifier &, const Packet &, uint32_t) = 0;
    virtual void dataSentTo(const NetworkIdentifier &, std::string_view) = 0;
    virtual void dataReceivedFrom(const NetworkIdentifier &, const std::string &) = 0;
};
static_assert(sizeof(IPacketObserver) == 24);
