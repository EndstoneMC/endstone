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

#include "bedrock/world/level/block/block_descriptor.h"

#include "endstone/detail/hook.h"

Block const *BlockDescriptor::tryGetBlockNoLogging() const
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&BlockDescriptor::tryGetBlockNoLogging, this);
}

namespace ScriptModuleMinecraft::ScriptBlockUtils {
BlockDescriptor createBlockDescriptor(
    std::string type, std::optional<std::unordered_map<std::string, std::variant<int, std::string, bool>>> states)
{
    return ENDSTONE_HOOK_CALL_ORIGINAL(&ScriptModuleMinecraft::ScriptBlockUtils::createBlockDescriptor, type, states);
}
}  // namespace ScriptModuleMinecraft::ScriptBlockUtils
