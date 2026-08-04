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

#include <cstdint>
#include <string>

#include "bedrock/bedrock.h"

// TODO(fixme): check the name - the cereal scope name is read from the binary, but its identity with
// this element could not be proven; only the layout is confirmed.
struct NetworkItemInstanceDescriptorData {
    int id;                       // +0
    std::int16_t stack_size;      // +4
    int aux_value;                // +8
    int block_runtime_id;         // +12
    std::string user_data_buffer;  // +16, an already-serialized NBT blob
};
BEDROCK_STATIC_ASSERT_SIZE(NetworkItemInstanceDescriptorData, 48, 40);
