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

#include "bedrock/util/pushable_by_entity_utility.h"

namespace {
thread_local PushableByEntityUtility::SkipPushBypassScope *current_skip_push_bypass = nullptr;
}  // namespace

PushableByEntityUtility::SkipPushBypassScope::SkipPushBypassScope(Actor &owner, Actor &other)
    : owner_(&owner), other_(&other), consumed_(false), previous_(current_skip_push_bypass)
{
    current_skip_push_bypass = this;
}

PushableByEntityUtility::SkipPushBypassScope::~SkipPushBypassScope()
{
    current_skip_push_bypass = previous_;
}

bool PushableByEntityUtility::SkipPushBypassScope::consume(Actor &owner, Actor &other)
{
    if (current_skip_push_bypass && !current_skip_push_bypass->consumed_ &&
        current_skip_push_bypass->owner_ == &owner && current_skip_push_bypass->other_ == &other) {
        current_skip_push_bypass->consumed_ = true;
        return true;
    }
    return false;
}
