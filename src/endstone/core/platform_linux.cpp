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

#include <fstream>

#include <fmt/format.h>

#include "endstone/core/platform.h"

namespace endstone::core {
std::string_view get_platform()
{
    return "Linux";
}

std::size_t get_proc_status(std::string_view key)
{
    std::ifstream file("/proc/self/status");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open /proc/self/status");
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.size() > key.size() && line.substr(0, key.size()) == key && line[key.size()] == ':') {
            return std::stoi(line.substr(key.size() + 1));
        }
    }

    throw std::runtime_error(fmt::format("Key {} not found in {}", key, "/proc/self/status"));
}

std::size_t get_thread_count()
{
    return get_proc_status("Threads");
}

std::size_t get_used_physical_memory()
{
    return get_proc_status("VmRSS") * 1024;
}

std::size_t get_total_virtual_memory()
{
    return get_proc_status("VmSize") * 1024;
}
}  // namespace endstone::core
#endif
