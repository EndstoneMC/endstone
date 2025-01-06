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

#include <string>

#include "bedrock/core/utility/observer.h"
#include "bedrock/platform/uuid.h"

namespace Social {
class MultiplayerServiceObserver : public Core::Observer<MultiplayerServiceObserver, Core::SingleThreadedLock> {
public:
    virtual void onInvalidPlayerJoinedLobby(mce::UUID const &uuid, std::string const &xuid) = 0;
    virtual void onUserDisconnectedBecauseConcurrentLogin(std::string const &id) = 0;
};
}  // namespace Social
