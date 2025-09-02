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

#include "bedrock/network/packet.h"
#include "bedrock/platform/uuid.h"
#include "bedrock/world/actor/player/serialized_skin.h"

class PlayerSkinPacket : public Packet {
public:
    mce::UUID uuid;
    SerializedSkin skin;
    std::string localized_new_skin_name;
    std::string localized_old_skin_name;
};
