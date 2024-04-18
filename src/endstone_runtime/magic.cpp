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

#include "bedrock/command/command_registry.h"
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
 * Symbols for CommandRegistry
 *
 * The values can be determined from CommandRegistry::describe(CommandParameterData const &)
 */
const CommandRegistry::Symbol CommandRegistry::Symbol::Int{0x100001};
const CommandRegistry::Symbol CommandRegistry::Symbol::Float{0x100003};
const CommandRegistry::Symbol CommandRegistry::Symbol::RelativeFloat{0x100004};
const CommandRegistry::Symbol CommandRegistry::Symbol::WildcardInt{0x100005};
const CommandRegistry::Symbol CommandRegistry::Symbol::Operator{0x100006};
const CommandRegistry::Symbol CommandRegistry::Symbol::CompareOperator{0x100007};
const CommandRegistry::Symbol CommandRegistry::Symbol::Selector{0x100008};
const CommandRegistry::Symbol CommandRegistry::Symbol::WildcardActorSelector{0x10000a};
const CommandRegistry::Symbol CommandRegistry::Symbol::FilePath{0x100011};
const CommandRegistry::Symbol CommandRegistry::Symbol::IntegerRange{0x100017};
const CommandRegistry::Symbol CommandRegistry::Symbol::EquipmentSlot{0x10002f};
const CommandRegistry::Symbol CommandRegistry::Symbol::String{0x100038};
const CommandRegistry::Symbol CommandRegistry::Symbol::Position{0x100040};
const CommandRegistry::Symbol CommandRegistry::Symbol::PositionFloat{0x100041};
const CommandRegistry::Symbol CommandRegistry::Symbol::Message{0x100044};
const CommandRegistry::Symbol CommandRegistry::Symbol::RawText{0x100046};
const CommandRegistry::Symbol CommandRegistry::Symbol::Json{0x10004a};
const CommandRegistry::Symbol CommandRegistry::Symbol::BlockState{0x100054};
const CommandRegistry::Symbol CommandRegistry::Symbol::Command{0x100057};

/**
 * CommandFlags
 */
const CommandFlag CommandFlag::None = {0};
const CommandFlag CommandFlag::HiddenFromBlock = {1 << 1};       // CommandRegistry::checkOriginCommandFlags
const CommandFlag CommandFlag::HiddenFromPlayer = {1 << 2};      // CommandRegistry::checkOriginCommandFlags
const CommandFlag CommandFlag::HiddenFromAutomation = {1 << 3};  // CommandRegistry::checkOriginCommandFlags
const CommandFlag CommandFlag::LocalOnly = {1 << 4};             // CommandRegistry::serializeAvailableCommands
const CommandFlag CommandFlag::Communication = {1 << 6};         // MinecraftCommands::requestCommandExecution
const CommandFlag CommandFlag::WithoutCheatEnabled = {1 << 7};   // Command::run
const CommandFlag CommandFlag::DisabledInEditor = {1 << 9};      // CommandRegistry::enabledInEditor

/**
 * Ids for VanillaDimensions
 *
 * See VanillaDimensions::fromSerializedInt
 */
const AutomaticID<Dimension, int> VanillaDimensions::Overworld{0};
const AutomaticID<Dimension, int> VanillaDimensions::Nether{1};
const AutomaticID<Dimension, int> VanillaDimensions::TheEnd{2};
