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

#ifdef __linux__

#include "endstone_runtime/hook.h"

#include <unordered_map>

#include <LIEF/LIEF.hpp>
#include <fmt/format.h>
#include <funchook/funchook.h>
#include <spdlog/spdlog.h>

#include "endstone_runtime/platform.h"

namespace endstone::hook {

namespace {
std::unordered_map<void *, void *> gOriginals;
}  // namespace

namespace detail {
void *get_original(void *detour)
{
    auto it = gOriginals.find(detour);
    if (it == gOriginals.end()) {
        return nullptr;
    }
    return it->second;
}
}  // namespace detail

void install()
{
    namespace ep = endstone::platform;

    // Find detours
    auto *module_base = ep::get_module_base();
    auto module_pathname = ep::get_module_pathname();

    std::unordered_map<std::string, void *> detours;
    {
        spdlog::debug("{}", module_pathname);
        auto elf = LIEF::ELF::Parser::parse(module_pathname);
        for (const auto &symbol : elf->static_symbols()) {
            if (!symbol.is_exported()) {
                continue;
            }

            if (symbol.binding() != LIEF::ELF::SYMBOL_BINDINGS::STB_GLOBAL) {
                continue;
            }

            auto name = symbol.name();
            auto offset = symbol.value();
            spdlog::debug("{} -> 0x{:x}", name, offset);
            auto *detour = static_cast<char *>(module_base) + offset;
            detours.emplace(symbol.name(), detour);
        }
    }

    // Find targets
    auto *executable_base = ep::get_executable_base();
    std::unordered_map<std::string, void *> targets;
    const auto executable_pathname = ep::get_executable_pathname() + "_symbols.debug";
    {
        auto elf = LIEF::ELF::Parser::parse(executable_pathname);
        for (const auto &symbol : elf->symbols()) {
            if (symbol.binding() != LIEF::ELF::SYMBOL_BINDINGS::STB_LOCAL) {
                continue;
            }

            auto name = symbol.name();
            auto offset = symbol.value();
            auto it = detours.find(name);
            if (it != detours.end()) {
                spdlog::debug("{} -> 0x{:x}", name, offset);
                auto *target = static_cast<char *>(executable_base) + offset;
                targets.emplace(name, target);
            }
        }
    }

    // Install hooks
    for (const auto &[name, detour] : detours) {
        auto it = targets.find(name);
        if (it != targets.end()) {
            void *target = it->second;
            void *original = target;

            spdlog::debug("{}: T = 0x{:p}", name, target);
            spdlog::debug("{}: D = 0x{:p}", name, detour);

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

            spdlog::debug("{}: O = 0x{:p}", name, original);
            gOriginals.emplace(detour, original);
        }
        else {
            throw std::runtime_error(fmt::format("Unable to find target function {} in the executable", name));
        }
    }
}

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

#endif
