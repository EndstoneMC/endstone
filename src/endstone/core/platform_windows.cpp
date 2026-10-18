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

#ifdef _WIN32

#include <Windows.h>
// ProcessSnapshot.h and Psapi.h must be included after Windows.h
#include <ProcessSnapshot.h>
#include <Psapi.h>

#include <string>
#include <string_view>
#include <system_error>

#include "endstone/core/platform.h"

namespace endstone::core {
std::string_view get_platform()
{
    return "Windows";
}

std::size_t get_thread_count()
{
    HPSS snapshot = nullptr;
    DWORD error = PssCaptureSnapshot(GetCurrentProcess(), PSS_CAPTURE_THREADS, 0, &snapshot);
    if (error != ERROR_SUCCESS) {
        throw std::system_error(static_cast<int>(error), std::system_category(), "PssCaptureSnapshot failed");
    }

    PSS_THREAD_INFORMATION info;
    error = PssQuerySnapshot(snapshot, PSS_QUERY_THREAD_INFORMATION, &info, sizeof(info));
    PssFreeSnapshot(GetCurrentProcess(), snapshot);
    if (error != ERROR_SUCCESS) {
        throw std::system_error(static_cast<int>(error), std::system_category(), "PssQuerySnapshot failed");
    }
    return info.ThreadsCaptured;
}
std::size_t get_used_physical_memory()
{
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.WorkingSetSize;
    }
    throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "GetProcessMemoryInfo failed");
}

std::size_t get_total_virtual_memory()
{
    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc))) {
        return pmc.PagefileUsage;
    }
    throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "GetProcessMemoryInfo failed");
}

}  // namespace endstone::core
#endif
