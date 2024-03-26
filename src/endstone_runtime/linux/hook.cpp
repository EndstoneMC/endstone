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

#include "endstone/detail/hook.h"

#include <unordered_map>

#include <LIEF/LIEF.hpp>
#include <funchook/funchook.h>
#include <spdlog/spdlog.h>

#include "endstone/detail/os.h"

namespace endstone::detail::hook {

std::unordered_map<std::string, void *> get_detours()
{
    // Find detours
    auto *module_base = os::get_module_base();
    auto module_pathname = os::get_module_pathname();

    std::unordered_map<std::string, void *> detours;

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

    return detours;
}

std::unordered_map<std::string, void *> get_targets()
{
    // Find targets
    auto *executable_base = os::get_executable_base();
    std::unordered_map<std::string, void *> targets;
    const auto executable_pathname = os::get_executable_pathname() + "_symbols.debug";

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

    return targets;
}
}  // namespace endstone::detail::hook

#endif
