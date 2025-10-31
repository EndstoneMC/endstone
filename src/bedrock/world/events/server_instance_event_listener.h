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

#include "bedrock/world/events/server_instance_events.h"

class Minecraft;
class Level;

class ServerInstanceEventListener {
public:
    using EventType = ServerInstanceNotificationEvent;
    virtual ~ServerInstanceEventListener() = default;
    virtual EventResult onServerInitializeStart(ServerInstance &)
    {
        return EventResult::KeepGoing;
    }
    virtual EventResult onServerInitializeEnd(ServerInstance &)
    {
        return EventResult::KeepGoing;
    }
    virtual EventResult onServerMinecraftInitialized(ServerInstance &, const Bedrock::NotNullNonOwnerPtr<Minecraft> &)
    {
        return EventResult::KeepGoing;
    }
    virtual EventResult onServerLevelInitialized(ServerInstance &, Level &)
    {
        return EventResult::KeepGoing;
    }
    virtual EventResult onServerUpdateStart(ServerInstance &)
    {
        return EventResult::KeepGoing;
    }
    virtual EventResult onServerUpdateEnd(ServerInstance &)
    {
        return EventResult::KeepGoing;
    }
    virtual EventResult onServerSuspend(ServerInstance &)
    {
        return EventResult::KeepGoing;
    }
    virtual EventResult onServerResume(ServerInstance &)
    {
        return EventResult::KeepGoing;
    }
    virtual EventResult onServerThreadStarted(ServerInstance &)
    {
        return EventResult::KeepGoing;
    }
    virtual EventResult onServerThreadStopped(ServerInstance &)
    {
        return EventResult::KeepGoing;
    }
    virtual EventResult onStartLeaveGame(ServerInstance &)
    {
        return EventResult::KeepGoing;
    }
    virtual EventResult onEvent(const EventType &)
    {
        return EventResult::KeepGoing;
    }
};
