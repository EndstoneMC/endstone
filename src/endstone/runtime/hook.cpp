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

#include <array>
#include <cstdint>
#include <format>
#include <string>
#include <string_view>
#include <system_error>
#include <unordered_map>

#include <spdlog/spdlog.h>

#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/mman.h>
#include <unistd.h>
#endif

#include "bedrock/symbol.h"
#include "endstone/core/platform.h"

namespace endstone::runtime::hook {
namespace {
void write_code_byte(unsigned char *target, unsigned char value)
{
#ifdef _WIN32
    DWORD old_protect;
    VirtualProtect(target, 1, PAGE_EXECUTE_READWRITE, &old_protect);
    *target = value;
    VirtualProtect(target, 1, old_protect, &old_protect);
    FlushInstructionCache(GetCurrentProcess(), target, 1);
#else
    const auto page_size = static_cast<std::uintptr_t>(sysconf(_SC_PAGESIZE));
    auto *page = reinterpret_cast<void *>(reinterpret_cast<std::uintptr_t>(target) & ~(page_size - 1));
    mprotect(page, page_size, PROT_READ | PROT_WRITE | PROT_EXEC);
    *target = value;
    mprotect(page, page_size, PROT_READ | PROT_EXEC);
#endif
}
}  // namespace

namespace details {
/**
 * @brief Mapping of hooked targets to their original implementations.
 *
 * Key   : the function pointer for the hooked (detour) target.
 * Value : the function pointer returned by the hooking framework;
 *         invoke this pointer to call the original function.
 */
using OriginalMap = std::unordered_map<void *, void *>;

static OriginalMap &originals()  // NOLINT(*-use-anonymous-namespace)
{
    static OriginalMap originals;
    return originals;
}

void *&get_original(void *target)
{
    const auto it = originals().find(target);
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
    auto *executable_base = get_executable_base();
    foreach_symbol([executable_base](const auto &key, auto offset) {
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
        [[nodiscard]] const char *name() const noexcept override { return "HookError"; }

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
    const auto &detours = details::get_detours();
    const auto &targets = details::get_targets();

    for (const auto &[name, detour] : detours) {
        if (auto it = targets.find(name); it != targets.end()) {
            void *target = it->second;
            void *original = target;

            funchook_t *hook = funchook_create();
            int status = funchook_prepare(hook, &original, detour);
            if (status != 0) {
                throw std::system_error(status, details::error_category(), std::format("Unable to hook {}", name));
            }

            status = funchook_install(hook, 0);
            if (status != 0) {
                throw std::system_error(status, details::error_category(), std::format("Unable to hook {}", name));
            }

            SPDLOG_DEBUG("{}: {} -> {} -> {}", name, target, detour, original);
            details::originals().emplace(target, original);
        }
        else {
            throw std::runtime_error(std::format("Unable to find target function for detour: {}.", name));
        }
    }

    // Only the server thread writes the random-tick seed and the next instruction reloads it, so a plain mov replaces
    // the locked xchg.
    static constexpr std::array unlocked_stores = {
        std::string_view{"LevelChunk::tickImpl::lightning_seed_store"},
        std::string_view{"LevelChunk::tickImpl::random_tick_seed_store"},
    };
    for (const auto name : unlocked_stores) {
        const auto it = targets.find(std::string(name));
        if (it == targets.end()) {
            throw std::runtime_error(std::format("Unable to find target instruction: {}.", name));
        }
        auto *target = static_cast<unsigned char *>(it->second);
        if (*target != 0x87) {
            throw std::runtime_error(std::format("Unexpected instruction at {}.", name));
        }
        write_code_byte(target, 0x89);
    }
}

}  // namespace endstone::runtime::hook
