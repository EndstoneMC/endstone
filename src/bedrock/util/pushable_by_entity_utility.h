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

#include "bedrock/bedrock.h"

class Actor;

namespace PushableByEntityUtility {
ENDSTONE_HOOK void push(Actor &owner, Actor &other, bool push_self_only);

class SkipPushBypassScope {
public:
    SkipPushBypassScope(Actor &owner, Actor &other);
    ~SkipPushBypassScope();

    SkipPushBypassScope(const SkipPushBypassScope &) = delete;
    SkipPushBypassScope &operator=(const SkipPushBypassScope &) = delete;

    static bool consume(Actor &owner, Actor &other);

private:
    Actor *owner_;
    Actor *other_;
    bool consumed_;
    SkipPushBypassScope *previous_;
};

ENDSTONE_HOOK bool skipPush(Actor &owner, Actor &other);
}  // namespace PushableByEntityUtility
