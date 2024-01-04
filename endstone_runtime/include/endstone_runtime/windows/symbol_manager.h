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

#pragma once

#ifdef _WIN32

#include <Windows.h>

#include <functional>
#include <mutex>
#include <random>
#include <string_view>

namespace endstone {
namespace sym {
class SymbolManager {
public:
    explicit SymbolManager(int sym_options = 0, const char *search_path = nullptr, bool invade_process = false);

    SymbolManager(const SymbolManager &) = delete;
    SymbolManager(SymbolManager &&other) noexcept;
    SymbolManager &operator=(const SymbolManager &) = delete;
    SymbolManager &operator=(SymbolManager &&) = delete;

    ~SymbolManager();

    /**
     * @brief Enumerates symbols in a specified module and invokes a callback function for each symbol found.
     *
     * This function enumerates symbols in the specified module and invokes the provided callback function for each
     * symbol found. The callback function should have the following signature: bool callback(string, size_t). The first
     * parameter of the callback function is the name of the symbol, and the second parameter is the offset of the
     * symbol from the base address of the module.
     *
     * @param module The handle to the module to enumerate symbols in.
     * @param mask A string specifying the symbol name mask to match.
     * @param callback The callback function to invoke for each symbol found.
     *
     * @throws std::system_error if GetModuleFileNameExW or SymLoadModuleExW fails.
     */
    void enumerate(HINSTANCE module, const char *mask, std::function<bool(const std::string &, size_t)> callback);

private:
    bool valid_ = true;
    HANDLE handle_;
    std::mutex mutex_;
};
}  // namespace sym
}  // namespace endstone

#endif  // _WIN32
