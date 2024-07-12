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

#include "bedrock/server/commands/command_registry.h"
#include "bedrock/world/level/dimension/vanilla_dimensions.h"

/**
 * NOTE for endstone developers:
 *
 * This file contains hard-coded magic values required for the Endstone to function.
 * Currently, there's no trivial way to determine these values during runtime.
 * Hence, these values are hard-coded for the time being.
 * These values need to be checked and updated on each game update.
 */

/**
 * CommandFlags
 */
const CommandFlag CommandFlag::None = {0};
const CommandFlag CommandFlag::HiddenFromBlock = {1 << 1};       // CommandRegistry::checkOriginCommandFlags
const CommandFlag CommandFlag::HiddenFromPlayer = {1 << 2};      // CommandRegistry::checkOriginCommandFlags
const CommandFlag CommandFlag::HiddenFromAutomation = {1 << 3};  // CommandRegistry::checkOriginCommandFlags
const CommandFlag CommandFlag::LocalOnly = {1 << 4};             // CommandRegistry::serializeAvailableCommands
const CommandFlag CommandFlag::Communication = {1 << 6};         // MinecraftCommands::requestCommandExecution
const CommandFlag CommandFlag::NotCheat = {1 << 7};              // Command::run
const CommandFlag CommandFlag::DisabledInEditor = {1 << 9};      // CommandRegistry::enabledInEditor

/**
 * CommandVersion
 */
const int CommandVersion::CurrentVersion = 39;
