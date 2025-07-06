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

#include <bitset>
#include <optional>

#include "bedrock/forward.h"

enum class AdvanceFrameResult : uint8_t {
    NoChange = 0,
    ObjectModified = 1,
    NotImplemented = 2,
};

namespace CorrectionType {
enum Name : size_t {
    Explicit = 0,
    Attributes = 1,
    Size = 2,
};

}  // namespace CorrectionType
using CorrectionTypeBitset = std::bitset<2UL>;

class Actor;

struct IMovementCorrection {
    virtual ~IMovementCorrection() = default;
    virtual AdvanceFrameResult getAdvanceFrameResult(const MovementDataExtractionUtility::SnapshotAccessor &) = 0;
    virtual void advanceLiveFrame(Actor &, std::optional<unsigned long>) = 0;
    [[nodiscard]] virtual CorrectionTypeBitset getCorrectionTypeBitset() const = 0;
    virtual void advanceFrame(EntityContext &) const = 0;
};
static_assert(sizeof(IMovementCorrection) == 8);
