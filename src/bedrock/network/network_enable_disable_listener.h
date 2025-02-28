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

#include "bedrock/core/utility/non_owner_pointer.h"
#include "bedrock/forward.h"

class NetworkEnableDisableListener {
    enum class State : int {
        Enabled = 0,
        Disabled = 1,
        Uninitialized = 2,
    };

public:
    NetworkEnableDisableListener(const Bedrock::NonOwnerPointer<AppPlatform> &);
    virtual ~NetworkEnableDisableListener() = default;

private:
    virtual void _onDisable() = 0;
    virtual void _onEnable() = 0;
    State state_;                                         // +8
    Bedrock::NonOwnerPointer<AppPlatform> app_platform_;  // +16
};
static_assert(sizeof(NetworkEnableDisableListener) == 40);
