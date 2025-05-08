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

#if defined(_WIN32) || defined(_WIN64)
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#include "endstone/server.h"

namespace endstone {
class Endstone {
public:
    /**
     * Gets the current Server singleton
     *
     * @return Server instance being run
     */
    static Server &getServer()
    {
        if (server_ == nullptr) {
#ifdef _WIN32
            auto handle = GetModuleHandle("endstone_runtime.dll");
#else
            auto handle = dlopen("libendstone_runtime.so", RTLD_LAZY);
#endif
            if (!handle) {
                throw std::runtime_error("Failed to load endstone runtime.");
            }

            using GetterFn = Server &(*)();
#ifdef _WIN32
            auto fn = reinterpret_cast<GetterFn>(GetProcAddress(handle, "endstone_get_server"));
#else
            auto fn = reinterpret_cast<GetterFn>(dlsym(handle, "endstone_get_server"));
#endif
            if (!fn) {
                throw std::runtime_error("Failed to find symbol endstone_get_server");
            }

            server_ = &fn();
            if (!server_) {
                throw std::runtime_error("Failed to get server instance.");
            }
        }

        return *server_;
    }

private:
    Endstone() = delete;

    static Server *server_;
};
}  // namespace endstone
