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

#include <unordered_map>

#include "bedrock/world/redstone/circuit/components/base_circuit_component.h"

class CircuitSceneGraph {
    using ComponentMap = std::unordered_map<BlockPos, std::unique_ptr<BaseCircuitComponent>>;

public:
    CircuitSceneGraph();
    BaseCircuitComponent *getBaseComponent(const BlockPos &pos)
    {
        auto it = all_components_.find(pos);
        if (it == all_components_.end()) {
            return nullptr;
        }
        return it->second.get();
    }

private:
    ComponentMap all_components_;  // +0
    //...
};
