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

#include "bedrock/network/packet.h"

struct RequestNetworkSettingsPacketPayload {
    int client_network_version;
};
BEDROCK_STATIC_ASSERT_SIZE(RequestNetworkSettingsPacketPayload, 4, 4);

class RequestNetworkSettingsPacket : public Packet {
public:
    RequestNetworkSettingsPacketPayload payload;  // +48 Windows, +44 Linux
    SerializationMode serialization_mode;         // +52 Windows, +48 Linux
};
BEDROCK_STATIC_ASSERT_SIZE(RequestNetworkSettingsPacket, 56, 56);
