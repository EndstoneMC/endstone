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

#include "bedrock/core/utility/pub_sub/connector.h"
#include "bedrock/world/level/dimension/dimension.h"

class IDimensionManagerConnector {
public:
    virtual Bedrock::PubSub::Connector<void(Dimension &)> &getOnNewDimensionCreatedConnector() = 0;
};

class DimensionManager : public IDimensionManagerConnector {
public:
    virtual ~DimensionManager();
    Bedrock::PubSub::Connector<void(Dimension &)> &getOnNewDimensionCreatedConnector() override;
    void shutdown();
    WeakRef<Dimension> getRandomDimension(Random &random);
    WeakRef<Dimension> getOrCreateDimension(DimensionType dimensionType);
    WeakRef<Dimension> getDimension(DimensionType dimensionType) const;
    bool hasDimensions() const;
    int getNumDimensions() const;
    void forEachDimension(std::function<bool(Dimension &)> callback)
    {
        for (auto &dimension : dimensions_ | std::views::values) {
            if (!callback(*dimension)) {
                return;
            }
        }
    }

    void forEachDimension(std::function<bool(const Dimension &)> callback) const
    {
        for (const auto &dimension : dimensions_ | std::views::values) {
            if (!callback(*dimension)) {
                return;
            }
        }
    }

private:
    std::unordered_map<AutomaticID<Dimension, int>, OwnerPtr<Dimension>> dimensions_;
};
