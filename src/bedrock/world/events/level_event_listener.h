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

class LevelEventListener {
    using EventType = class LevelNotificationEvent;

public:
    virtual ~LevelEventListener();
    virtual EventResult onLevelInitialized(Level &);
    virtual EventResult onLevelAddedPlayer(Player &);
    virtual EventResult onLevelRemovedPlayer(Player &);
    virtual EventResult onLevelRemovedActor(Actor &);
    virtual EventResult onLevelTick(Level &);
    virtual EventResult onLevelTickStart(Level &);
    virtual EventResult onLevelTickEnd(Level &);
    virtual EventResult onLevelWeatherChange(const std::string &, bool, bool, bool, bool);
    virtual EventResult onEvent(const EventType &);
};
