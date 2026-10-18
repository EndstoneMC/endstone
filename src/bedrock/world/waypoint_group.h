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

#include <cstddef>

#include "bedrock/platform/uuid.h"

class WaypointGroup {
public:
    class WaypointHandle {
    public:
        struct Hasher {
            std::size_t operator()(const WaypointHandle &handle) const;
        };
        bool operator==(const WaypointHandle &other) const;
        bool operator<(const WaypointHandle &other) const;
        WaypointHandle();
        WaypointHandle(const mce::UUID &);  // NOLINT(*-explicit-constructor)
        mce::UUID uuid;
    };
    virtual ~WaypointGroup();
    [[nodiscard]] virtual bool has(const WaypointHandle &) const = 0;
    virtual bool remove(const WaypointHandle &) = 0;
};
