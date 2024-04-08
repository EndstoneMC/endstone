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

/**
 * NOTE for endstone developers:
 *
 * This file contains hard-coded magic values required for the Endstone to function.
 * Currently, there's no trivial way to determine these values during runtime.
 * Hence, these values are hard-coded for the time being.
 * These values need to be checked and updated on each game update.
 */

// Check CommandRegistry::describe(CommandParameterData const &) for symbol ids
const CommandRegistry::Symbol CommandRegistry::Symbol::INT{0x100001};
const CommandRegistry::Symbol CommandRegistry::Symbol::FLOAT{0x100003};
const CommandRegistry::Symbol CommandRegistry::Symbol::RELATIVE_FLOAT{0x100004};
const CommandRegistry::Symbol CommandRegistry::Symbol::WILDCARD_INT{0x100005};
const CommandRegistry::Symbol CommandRegistry::Symbol::OPERATOR{0x100006};
const CommandRegistry::Symbol CommandRegistry::Symbol::COMPARE_OPERATOR{0x100007};
const CommandRegistry::Symbol CommandRegistry::Symbol::SELECTOR{0x100008};
const CommandRegistry::Symbol CommandRegistry::Symbol::WILDCARD_ACTOR_SELECTOR{0x10000a};
const CommandRegistry::Symbol CommandRegistry::Symbol::FILE_PATH{0x100011};
const CommandRegistry::Symbol CommandRegistry::Symbol::INTEGER_RANGE{0x100017};
const CommandRegistry::Symbol CommandRegistry::Symbol::EQUIPMENT_SLOT{0x10002f};
const CommandRegistry::Symbol CommandRegistry::Symbol::STRING{0x100038};
const CommandRegistry::Symbol CommandRegistry::Symbol::POSITION{0x100040};
const CommandRegistry::Symbol CommandRegistry::Symbol::POSITION_FLOAT{0x100041};
const CommandRegistry::Symbol CommandRegistry::Symbol::MESSAGE{0x100044};
const CommandRegistry::Symbol CommandRegistry::Symbol::RAW_TEXT{0x100046};
const CommandRegistry::Symbol CommandRegistry::Symbol::JSON{0x10004a};
const CommandRegistry::Symbol CommandRegistry::Symbol::BLOCK_STATE{0x100054};
const CommandRegistry::Symbol CommandRegistry::Symbol::COMMAND{0x100057};
