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

#include <funchook.h>

#include "endstone_runtime/hook.h"

namespace endstone::hook {
const std::error_category &hook_error_category() noexcept
{
    static const class HookErrorCategory : public std::error_category {
    public:
        [[nodiscard]] const char *name() const noexcept override
        {
            return "HookError";
        }

        [[nodiscard]] std::string message(int err_val) const override
        {
            switch (err_val) {
            case FUNCHOOK_ERROR_INTERNAL_ERROR:
                return "FUNCHOOK_ERROR_INTERNAL_ERROR";
            case FUNCHOOK_ERROR_SUCCESS:
                return "FUNCHOOK_ERROR_SUCCESS";
            case FUNCHOOK_ERROR_OUT_OF_MEMORY:
                return "FUNCHOOK_ERROR_OUT_OF_MEMORY";
            case FUNCHOOK_ERROR_ALREADY_INSTALLED:
                return "FUNCHOOK_ERROR_ALREADY_INSTALLED";
            case FUNCHOOK_ERROR_DISASSEMBLY:
                return "FUNCHOOK_ERROR_DISASSEMBLY";
            case FUNCHOOK_ERROR_IP_RELATIVE_OFFSET:
                return "FUNCHOOK_ERROR_IP_RELATIVE_OFFSET";
            case FUNCHOOK_ERROR_CANNOT_FIX_IP_RELATIVE:
                return "FUNCHOOK_ERROR_CANNOT_FIX_IP_RELATIVE";
            case FUNCHOOK_ERROR_FOUND_BACK_JUMP:
                return "FUNCHOOK_ERROR_FOUND_BACK_JUMP";
            case FUNCHOOK_ERROR_TOO_SHORT_INSTRUCTIONS:
                return "FUNCHOOK_ERROR_TOO_SHORT_INSTRUCTIONS";
            case FUNCHOOK_ERROR_MEMORY_ALLOCATION:
                return "FUNCHOOK_ERROR_MEMORY_ALLOCATION";
            case FUNCHOOK_ERROR_MEMORY_FUNCTION:
                return "FUNCHOOK_ERROR_MEMORY_FUNCTION";
            case FUNCHOOK_ERROR_NOT_INSTALLED:
                return "FUNCHOOK_ERROR_NOT_INSTALLED";
            case FUNCHOOK_ERROR_NO_AVAILABLE_REGISTERS:
                return "FUNCHOOK_ERROR_NO_AVAILABLE_REGISTERS";
            default:
                return "Unknown error.";
            }
        }
    } CATEGORY;
    return CATEGORY;
}
}  // namespace endstone::hook
