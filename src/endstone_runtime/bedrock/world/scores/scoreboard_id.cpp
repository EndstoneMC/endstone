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

#include "bedrock/world/scores/scoreboard_id.h"

#include "bedrock/world/scores/identity_definition.h"

IdentityDefinition const &ScoreboardId::getIdentityDef() const
{
    if (identity_def) {
        return *identity_def;
    }
    return IdentityDefinition::Invalid;
}

bool ScoreboardId::isValid() const
{
    return raw_id != INVALID.raw_id;
}

bool ScoreboardId::operator==(const ScoreboardId &other) const
{
    return raw_id == other.raw_id;
}

std::size_t ScoreboardId::getHash() const
{
    static std::hash<ActorUniqueID> hasher;
    return hasher(ActorUniqueID(raw_id));
}
