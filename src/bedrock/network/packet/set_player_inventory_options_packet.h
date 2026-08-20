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

#include "bedrock/bedrock.h"
#include "bedrock/network/packet.h"
#include "bedrock/world/inventory/inventory_options.h"

struct SetPlayerInventoryOptionsPacketPayload {
    InventoryOptions inventory_options;
};
BEDROCK_STATIC_ASSERT_SIZE(SetPlayerInventoryOptionsPacketPayload, 20, 20);

class SetPlayerInventoryOptionsPacket : public Packet {
public:
    SetPlayerInventoryOptionsPacketPayload payload;
    SerializationMode serialization_mode;
};
BEDROCK_STATIC_ASSERT_SIZE(SetPlayerInventoryOptionsPacket, 72, 72);
