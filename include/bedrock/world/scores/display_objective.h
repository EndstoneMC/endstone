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

#include "bedrock/world/scores/objective.h"
#include "bedrock/world/scores/objective_sort_order.h"

class DisplayObjective {
public:
    [[nodiscard]] bool isDisplaying(const Objective &objective) const
    {
        return objective_ == &objective;
    }

    [[nodiscard]] bool isValid() const
    {
        return objective_ != nullptr;
    }

    [[nodiscard]] const Objective &getObjective() const
    {
        return *objective_;
    }

    [[nodiscard]] ObjectiveSortOrder getSortOrder() const
    {
        return sort_order_;
    }

private:
    const Objective *objective_;
    ObjectiveSortOrder sort_order_;
};
