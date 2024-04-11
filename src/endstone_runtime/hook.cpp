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

#include "endstone/detail/hook.h"

#include <string>
#include <system_error>
#include <unordered_map>

#include <funchook/funchook.h>
#include <spdlog/spdlog.h>

namespace endstone::detail::hook {

namespace {
std::unordered_map<void *, void *> gOriginalsByDetour;
std::unordered_map<std::string, void *> gOriginalsByName;
}  // namespace

void *get_original(void *detour)
{
    auto it = gOriginalsByDetour.find(detour);
    if (it == gOriginalsByDetour.end()) {
        throw std::runtime_error(fmt::format("No original function can be found for address {}", detour));
    }
    return it->second;
}

void *get_original(const std::string &name)
{
    auto it = gOriginalsByName.find(name);
    if (it == gOriginalsByName.end()) {
        throw std::runtime_error(fmt::format("No original function can be found for name {}", name));
    }
    return it->second;
}

void install()
{
    auto detours = get_detours();
    auto targets = get_targets();

    for (const auto &[name, detour] : detours) {
        auto it = targets.find(name);
        if (it != targets.end()) {
            void *target = it->second;
            void *original = target;

            funchook_t *hook = funchook_create();
            int status;
            status = funchook_prepare(hook, &original, detour);
            if (status != 0) {
                throw std::system_error(status, hook_error_category());
            }

            status = funchook_install(hook, 0);
            if (status != 0) {
                throw std::system_error(status, hook_error_category());
            }

            spdlog::debug("{}: {} -> {} -> {}", name, target, detour, original);
            gOriginalsByDetour.emplace(detour, original);
            gOriginalsByName.emplace(name, original);
        }
        else {
            throw std::runtime_error(fmt::format("Unable to find target function {} to hook.", name));
        }
    }
}

const std::error_category &hook_error_category()
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
    } category;
    return category;
}
}  // namespace endstone::detail::hook
