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
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

// Helper function to convert a Python dictionary to a Windows-compatible environment block
std::string convert_environment(const std::map<std::string, std::string> &env)
{
    std::string env_block;
    for (const auto &[key, value] : env) {
        env_block += key + "=" + value + '\0';
    }
    env_block += '\0';  // Double null-terminate
    return env_block;
}

// Function to start a process with DLL injection
int start_process_with_dll(const std::string &process_path, const std::string &dll_path, const std::string &cwd = "",
                           const std::map<std::string, std::string> &env = {})
{
    STARTUPINFOA si{};
    PROCESS_INFORMATION pi{};
    si.cb = sizeof(STARTUPINFOA);

    // Convert C++ string (working directory) to C-style string
    LPSTR lpCurrentDirectory = cwd.empty() ? nullptr : const_cast<char *>(cwd.c_str());

    // Convert environment dictionary to a Windows-compatible environment block
    std::string env_block_string = convert_environment(env);
    LPVOID lpEnvironment = env.empty() ? nullptr : (LPVOID)env_block_string.c_str();

    // Call the Detours library API
    BOOL result = DetourCreateProcessWithDllExA(process_path.c_str(),       // Path of the executable to launch
                                                nullptr,                    // Command line (empty/null)
                                                nullptr,                    // Process security attributes
                                                nullptr,                    // Thread security attributes
                                                false,                      // Inherit handles
                                                CREATE_DEFAULT_ERROR_MODE,  // Creation flags
                                                lpEnvironment,              // Environment block
                                                lpCurrentDirectory,         // Current directory
                                                &si,                        // Startup info
                                                &pi,                        // Process information
                                                dll_path.c_str(),           // Path to the DLL to inject
                                                nullptr                     // Use default thread attributes
    );

    // Check if the process creation was successful
    if (!result) {
        DWORD error = GetLastError();
        throw std::runtime_error("Error: Failed to create process with DLL. Error code: " + std::to_string(error));
    }

    // Wait for the process to exit
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Get the exit code of the process
    DWORD exit_code;
    if (!GetExitCodeProcess(pi.hProcess, &exit_code)) {
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        throw std::runtime_error("Error: Failed to retrieve process exit code. Error code: " +
                                 std::to_string(GetLastError()));
    }

    // Close process and thread handles
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return static_cast<int>(exit_code);
}

PYBIND11_MODULE(winext, m)
{
    m.doc() = "Windows-specific Python extension for the Endstone project";
    // Bind the function
    m.def("start_process_with_dll", &start_process_with_dll, py::arg("process_path"), py::arg("dll_path"),
          py::arg("cwd") = "", py::arg("env") = std::map<std::string, std::string>{},
          "Start a process with a DLL injected and return the exit code of the process.\n");
}
