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
#include "bedrock/network/raknet/types.h"

/**
 * https://github.com/facebookarchive/RakNet/blob/master/Source/RakNetSocket2.h
 */

namespace RakNet {

using RNS2SendResult = int;
using RNS2Socket = int;

struct RNS2_SendParameters {  // NOLINT
    RNS2_SendParameters()
    {
        ttl = 0;
    }
    char *data;
    int length;
    SystemAddress system_address;
    int ttl;
};

class RNS2_Windows_Linux_360 {  // NOLINT
protected:
    // NOLINTNEXTLINE
    BEDROCK_API static RNS2SendResult Send_Windows_Linux_360NoVDP(RNS2Socket socket,
                                                                  RNS2_SendParameters *send_parameters,
                                                                  const char *file, unsigned int line);
};

}  // namespace RakNet
