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

#include "endstone/runtime/hook.h"

#include <funchook.h>

#include <string>
#include <system_error>
#include <unordered_map>

#include <entt/core/hashed_string.hpp>
#include <spdlog/spdlog.h>

#include "bedrock/symbol.h"
#include "endstone/detail/platform.h"

namespace endstone::hook {
namespace details {
using OriginalMap = std::unordered_map<entt::hashed_string::hash_type, void *>;
static OriginalMap &originals()  // NOLINT(*-use-anonymous-namespace)
{
    static OriginalMap originals;
    return originals;
}

void *&get_original(entt::hashed_string::hash_type name)
{
    const auto it = originals().find(name);
    if (it == originals().end()) {
        throw std::runtime_error("original function not found");
    }
    return it->second;
}

const std::unordered_map<std::string, void *> &get_targets()
{
    static std::unordered_map<std::string, void *> targets;
    if (!targets.empty()) {
        return targets;
    }
    auto *executable_base = detail::get_executable_base();
    detail::foreach_symbol([executable_base](const auto &key, auto offset) {
        SPDLOG_DEBUG("T: {} -> 0x{:x}", key, offset);
        auto *target = static_cast<char *>(executable_base) + offset;
        targets.emplace(key, target);
    });
    return targets;
}

const std::error_category &error_category()
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
}  // namespace details

void install()
{
    auto detours = details::get_detours();
    const auto &targets = details::get_targets();

    detours.erase("endstone_getServer");
    for (const auto &[name, detour] : detours) {
        if (auto it = targets.find(name); it != targets.end()) {
            void *target = it->second;
            void *original = target;

            funchook_t *hook = funchook_create();
            int status = funchook_prepare(hook, &original, detour);
            if (status != 0) {
                throw std::system_error(status, details::error_category(), fmt::format("Unable to hook {}", name));
            }

            status = funchook_install(hook, 0);
            if (status != 0) {
                throw std::system_error(status, details::error_category(), fmt::format("Unable to hook {}", name));
            }

            SPDLOG_DEBUG("{}: {} -> {} -> {}", name, target, detour, original);
            details::originals().emplace(entt::hashed_string::value(name.c_str()), original);
        }
        else {
            throw std::runtime_error(fmt::format("Unable to find target function for detour: {}.", name));
        }
    }
}

}  // namespace endstone::hook
