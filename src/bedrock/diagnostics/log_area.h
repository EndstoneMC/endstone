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

namespace BedrockLog {
enum LogCategory : unsigned int {
    LogArea = 0,
    LogWorldGen = 1,
    LogLoot = 2,
    LogRender = 3,
    LogStructure = 4,
    LogUI = 5,
    LogOnline = 6,
    NumLogCategories = 7,
};

enum LogRule : unsigned int {
    DefaultRules = 0,
    ClientAndServer = 1,
};
}  // namespace BedrockLog

enum class LogAreaID {
    All,
    Platform,
    Entity,
    Database,
    GUI,
    System,
    Network,
    Render,
    Memory,
    Animation,
    Input,
    Level,
    Server,
    DLC,
    Physics,
    File,
    Storage,
    Realms,
    RealmsAPI,
    XboxLive,
    UserManager,
    XsAPI,
    Perf,
    Telemetry,
    Blocks,
    RakNet,
    Gameface,
    Sound,
    Interactive,
    Scripting,
    PlayFab,
    Automation,
    Persona,
    Texture,
    AssetPackages,
    Items,
    Services,
    Volumes,
    LootTable,
    Sidebar,
    Localization,
    Movement,
    LiveEvents,
    Editor,
    LevelTransition,
    Unknown,
    Store,
    World,
    Messaging,
    NetherNet,
    Serialization,
    DisconnectionScreen,
    NumLogAreas,
};
