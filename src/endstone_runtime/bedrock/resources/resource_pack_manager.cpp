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

#include "bedrock/resources/resource_pack_manager.h"

ResourcePackStack const &ResourcePackManager::getStack(ResourcePackStackType stack_type) const
{
    switch (stack_type) {
    case ResourcePackStackType::LEVEL:
        return *level_stack_;
    case ResourcePackStackType::ADDON:
        return *addon_stack_;
    case ResourcePackStackType::GLOBAL:
        return *global_stack_;
    case ResourcePackStackType::TREATMENT:
        return *treatment_stack_;
    case ResourcePackStackType::BASE_GAME:
        return *base_game_stack_;
    default:
        throw std::runtime_error("Invalid stack type");
    }
}

bool ResourcePackManager::setStack(std::unique_ptr<ResourcePackStack> stack, ResourcePackStackType stack_type,
                                   bool compose)
{
    switch (stack_type) {
    case ResourcePackStackType::LEVEL:
        level_stack_ = std::move(stack);
        break;
    case ResourcePackStackType::ADDON:
        addon_stack_ = std::move(stack);
        break;
    case ResourcePackStackType::GLOBAL:
        global_stack_ = std::move(stack);
        break;
    case ResourcePackStackType::TREATMENT:
        treatment_stack_ = std::move(stack);
        break;
    case ResourcePackStackType::BASE_GAME:
        base_game_stack_ = std::move(stack);
        break;
    default:
        return false;
    }

    if (compose) {
        throw std::runtime_error("compose=true is not supported yet");
    }
    else {
        pending_restack_ = true;
    }
    return true;
}
