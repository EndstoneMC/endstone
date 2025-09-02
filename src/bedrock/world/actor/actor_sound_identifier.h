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

#include "actor.h"
#include "bedrock/core/utility/non_owner_pointer.h"

class ActorFetcher;
class ActorSoundIdentifier {
public:
    ActorSoundIdentifier() : identifier_(EMPTY_IDENTIFIER) {}

private:
    inline static ActorDefinitionIdentifier EMPTY_IDENTIFIER;
    const ActorDefinitionIdentifier &identifier_;
    Bedrock::NonOwnerPointer<const ActorFetcher> actor_fetcher_;
    ActorUniqueID actor_id_ = ActorUniqueID::INVALID_ID;
    bool is_baby_ = false;
};
static_assert(sizeof(ActorSoundIdentifier) == 48);
