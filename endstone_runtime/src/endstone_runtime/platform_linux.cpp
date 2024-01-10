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

#include <climits>
#include <fstream>

#include <LIEF/LIEF.hpp>
#include <fmt/format.h>
#include <spdlog/spdlog.h>

#include "endstone_runtime/platform.h"

namespace endstone::platform {

namespace {
struct MmapRegion {
    size_t begin;
    size_t end;
    char perms[4 + 1];  // rwxp(4) + \0(1)
    size_t offset;
    char device[255 + 1];
    size_t inode;
    char pathname[PATH_MAX + 1];
};

std::string get_filename(std::string pathname)
{
    size_t pos = pathname.find_last_of('/');
    if (pos != std::string::npos) {
        return pathname.substr(pos + 1);
    }
    return pathname;
}

void enumerate_mmap(std::function<bool(const MmapRegion &)> callback)
{
    std::ifstream file("/proc/self/maps");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open /proc/self/maps");
    }

    for (std::string line; std::getline(file, line);) {
        MmapRegion region;
        int r = sscanf(line.c_str(), "%lx-%lx %4s %lx %10s %ld %s", &region.begin, &region.end, region.perms,
                       &region.offset, region.device, &region.inode, region.pathname);
        if (r != 7) {
            continue;
        }

        if (!callback(region)) {
            break;
        }
    }
}
}  // namespace

void *get_module_base(const char *module_name)
{
    void *module_base;
    enumerate_mmap([&](const MmapRegion &region) -> bool {
        if ((module_name == nullptr) || get_filename(region.pathname) == module_name) {
            module_base = reinterpret_cast<void *>(region.begin);  // NOLINT(*-no-int-to-ptr)
            return false;                                          // stop enumeration
        }

        return true;
    });

    if (!module_base) {
        throw std::runtime_error(fmt::format("Failed to find module with name {}", module_name));
    }

    return module_base;
}

std::string get_module_pathname(const char *module_name)
{
    std::string pathname;
    enumerate_mmap([&](const MmapRegion &region) -> bool {
        if ((module_name == nullptr) || get_filename(region.pathname) == module_name) {
            pathname = region.pathname;
            return false;  // stop enumeration
        }

        return true;
    });

    if (pathname.empty()) {
        throw std::runtime_error(fmt::format("Failed to find module with name {}", module_name));
    }

    return pathname;
}

void enumerate_symbols(const char *path, std::function<bool(const std::string &, size_t)> callback)
{
    spdlog::info("{}", path);
    auto elf = LIEF::ELF::Parser::parse(path);
    for (const auto &symbol : elf->static_symbols()) {
        if (!symbol.is_exported() || symbol.binding() != LIEF::ELF::SYMBOL_BINDINGS::STB_GLOBAL) {
            continue;
        }

        if (!callback(symbol.name(), symbol.value())) {
            break;
        }
    }
}

}  // namespace endstone::platform

#endif
