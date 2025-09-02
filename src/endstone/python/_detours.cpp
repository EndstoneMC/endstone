// Copyright (c) 2024, The Endstone Project. (https://endstone.dev) All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the license at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <windows.h>
// detours.h must be included after windows.h
#include <detours.h>

#include <string>
#include <system_error>
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

HANDLE get_handle(py::object object, const char *name)
{
    if (object.is_none()) {
        return nullptr;
    }
    auto attr = object.attr(name);
    if (attr.is_none()) {
        return nullptr;
    }
    return reinterpret_cast<HANDLE>(object.attr(name).cast<std::size_t>());
}

py::tuple CreateProcessWithDllEx(
    // positional-only args, like _winapi.CreateProcess
    std::optional<std::wstring> application_name, std::optional<std::wstring> command_line, py::object proc_attrs_obj,
    py::object thread_attrs_obj, bool inherit_handles, unsigned long creation_flags,
    std::map<std::wstring, std::wstring> env, std::optional<std::wstring> current_directory,
    py::object startup_info_obj,
    // keyword-only
    const std::string &dll_name)
{
    STARTUPINFOEXW si;
    ZeroMemory(&si, sizeof(si));
    si.StartupInfo.cb = sizeof(si);
    si.StartupInfo.dwFlags = startup_info_obj.attr("dwFlags").cast<DWORD>();
    si.StartupInfo.wShowWindow = startup_info_obj.attr("wShowWindow").cast<WORD>();
    si.StartupInfo.hStdInput = get_handle(startup_info_obj, "hStdInput");
    si.StartupInfo.hStdOutput = get_handle(startup_info_obj, "hStdOutput");
    si.StartupInfo.hStdError = get_handle(startup_info_obj, "hStdError");

    LPVOID lpEnvironment = nullptr;
    std::wstring env_block;
    if (!env.empty()) {
        for (const auto &[key, value] : env) {
            env_block += key + L"=" + value + L'\0';
        }
        env_block += L'\0';  // Double null-terminate
        lpEnvironment = env_block.data();
    }

    std::wstring app, cmd, cwd;
    LPCWSTR lpAppName = nullptr;
    LPCWSTR lpCwd = nullptr;
    LPWSTR lpCmdLine = nullptr;

    if (application_name) {
        app = application_name.value();
        lpAppName = app.c_str();
    }
    if (command_line) {
        cmd = command_line.value();
        lpCmdLine = &cmd[0];  // API expects mutable LPSTR
    }
    if (current_directory) {
        cwd = current_directory.value();
        lpCwd = cwd.c_str();
    }

    PROCESS_INFORMATION pi;
    ZeroMemory(&pi, sizeof(pi));

    BOOL ok = DetourCreateProcessWithDllExW(
        lpAppName,                                                                   // Path of the executable to launch
        lpCmdLine,                                                                   // Command line (empty/null)
        nullptr,                                                                     // Process security attributes
        nullptr,                                                                     // Thread security attributes
        inherit_handles ? TRUE : FALSE,                                              // Inherit handles
        creation_flags | EXTENDED_STARTUPINFO_PRESENT | CREATE_UNICODE_ENVIRONMENT,  // Creation flags
        lpEnvironment,                                                               // Environment block
        lpCwd,                                                                       // Current directory
        reinterpret_cast<LPSTARTUPINFOW>(&si),                                       // Startup info
        &pi,                                                                         // Process information
        dll_name.c_str(),                                                            // Path to the DLL to inject
        nullptr                                                                      // Use default thread attributes
    );

    if (!ok) {
        auto err = GetLastError();
        throw std::system_error(err, std::system_category(),
                                "DetourCreateProcessWithDllExA failed: " + std::to_string(err));
    }

    return py::make_tuple(reinterpret_cast<std::uintptr_t>(pi.hProcess), reinterpret_cast<std::uintptr_t>(pi.hThread),
                          pi.dwProcessId, pi.dwThreadId);
}

PYBIND11_MODULE(_detours, m)
{
    m.def("CreateProcessWithDllEx", &CreateProcessWithDllEx,
          // positional‐only, matching _winapi.CreateProcess
          py::arg("application_name") = py::none(), py::arg("command_line") = py::none(),
          py::arg("proc_attrs") = py::none(), py::arg("thread_attrs") = py::none(), py::arg("inherit_handles"),
          py::arg("creation_flags"), py::arg("env_mapping") = std::map<std::string, std::string>{},
          py::arg("current_directory") = py::none(), py::arg("startup_info"), py::kw_only(),
          // extra keyword‐only args
          py::arg("dll_name"));
}
