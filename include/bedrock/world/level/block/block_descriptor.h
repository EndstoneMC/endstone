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

#include <optional>
#include <string>
#include <unordered_map>
#include <variant>

#include "bedrock/bedrock.h"
#include "bedrock/core/spin_lock.h"
#include "bedrock/world/level/block/block.h"

class BlockDescriptor {
    enum class CompareType;

public:
    ENDSTONE_HOOK [[nodiscard]] Block const *tryGetBlockNoLogging() const;

private:
    void *unknown1_;                       // +0
    HashedString block_name_;              // +8
    bool is_complex_alias_block_;          // +56
    std::shared_ptr<void *> tags_;         // +64  void*=ExpressionNode
    std::vector<void *> states_;           // +80  void*=BlockDescriptor::State
    std::vector<void *> resolved_states_;  // +104 void*=BlockDescriptor::State
    CompareType compare_type_;             // +128
    bool content_log_on_error_;            // +132
    Block *block_;                         // +140
    bool valid_;                           // +144
    bool is_deferred_;                     // +145
    SpinLock lock_;                        // +152
};

namespace ScriptModuleMinecraft::ScriptBlockUtils {
ENDSTONE_HOOK BlockDescriptor createBlockDescriptor(
    std::string type, std::optional<std::unordered_map<std::string, std::variant<int, std::string, bool>>> states);

}  // namespace ScriptModuleMinecraft::ScriptBlockUtils
