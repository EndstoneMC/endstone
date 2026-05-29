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

enum class MolangVersion : std::int16_t {
    Invalid = -1,
    BeforeVersioning = 0,
    Initial = 1,
    FixedItemRemainingUseDurationQuery = 2,
    ExpressionErrorMessages = 3,
    UnexpectedOperatorErrors = 4,
    ConditionalOperatorAssociativity = 5,
    ComparisonAndLogicalOperatorPrecedence = 6,
    DivideByNegativeValue = 7,
    FixedCapeFlapAmountQuery = 8,
    QueryBlockPropertyRenamedToState = 9,
    DeprecateOldBlockQueryNames = 10,
    DeprecatedSnifferAndCamelQueries = 11,
    LeafSupportingInFirstSolidBlockBelow = 12,
    CarryingBlockQueryAllActors = 13,
    NumValidVersions = 14,
    Latest = 13,
    HardcodedMolang = 13,
};
