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

class Packet {
public:
    virtual ~Packet() = default;

private:
    int unknown1_{2};           // +8
    int unknown2_{1};           // +12
    int unknown3_{0};           // +16
    std::int64_t unknown4_{0};  // +24
    std::int64_t unknown5_{0};  // +32
    int unknown6_{0};           // +40
};
static_assert(sizeof(Packet) == 48);
