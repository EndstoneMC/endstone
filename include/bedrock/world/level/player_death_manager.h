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

#include "bedrock/core/utility/pub_sub/publisher.h"
#include "bedrock/core/utility/pub_sub/thread_model.h"

class IPlayerDeathManagerProxy : public Bedrock::EnableNonOwnerReferences {
public:
    ~IPlayerDeathManagerProxy() override = 0;
    [[nodiscard]] virtual Actor *fetchActor(ActorUniqueID) const = 0;
    [[nodiscard]] virtual bool shouldShowDeathMessages() const = 0;
};

class PlayerDeathManagerBase {  // TODO(fixme): figure out the name
public:
    virtual ~PlayerDeathManagerBase() = default;
};

class PlayerDeathManager
    : public PlayerDeathManagerBase,
      public Bedrock::PubSub::Publisher<void(Player &), Bedrock::PubSub::ThreadModel::MultiThreaded> {
public:
    [[nodiscard]] gsl::not_null<IPlayerDeathManagerProxy *> getPlayerDeathManagerProxy() const
    {
        return proxy_.get();
    }

    void resetPacketSender()  // Endstone
    {
        sender_.reset();
    }

private:
    std::unique_ptr<IPlayerDeathManagerProxy> proxy_;  // +136
    Bedrock::NonOwnerPointer<PacketSender> sender_;    // +144
};
BEDROCK_STATIC_ASSERT_SIZE(PlayerDeathManager, 168, 128);
