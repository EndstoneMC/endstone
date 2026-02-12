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

#include <stdexcept>

#if defined(_WIN32) || defined(_WIN64)
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#else
#include <dlfcn.h>
#endif

#define ENDSTONE_STRINGIFY(x) #x
#define ENDSTONE_TOSTRING(x)  ENDSTONE_STRINGIFY(x)

#define ENDSTONE_VERSION_MAJOR 0
#define ENDSTONE_VERSION_MINOR 11
#define ENDSTONE_VERSION_PATCH 0

#define NETWORK_PROTOCOL_VERSION 924

#define ENDSTONE_API_VERSION ENDSTONE_TOSTRING(ENDSTONE_VERSION_MAJOR) "." ENDSTONE_TOSTRING(ENDSTONE_VERSION_MINOR)

#ifndef ENDSTONE_VERSION
#define ENDSTONE_VERSION                      \
    ENDSTONE_TOSTRING(ENDSTONE_VERSION_MAJOR) \
    "." ENDSTONE_TOSTRING(ENDSTONE_VERSION_MINOR) "." ENDSTONE_TOSTRING(ENDSTONE_VERSION_PATCH)
#endif

#define MINECRAFT_VERSION_MAJOR 1
#define MINECRAFT_VERSION_MINOR 26
#define MINECRAFT_VERSION_PATCH 0

#ifndef MINECRAFT_VERSION
#define MINECRAFT_VERSION ENDSTONE_TOSTRING(MINECRAFT_VERSION_MINOR) "." ENDSTONE_TOSTRING(MINECRAFT_VERSION_PATCH)
#endif

namespace endstone {
class Server;

namespace detail {
/**
 * @brief Cast a function pointer to void pointer
 *
 * @tparam Return The return type of the function.
 * @tparam Args The argument types of the function.
 *
 * @param fp The function pointer to be casted.
 *
 * @return A void pointer to the function.
 */
template <typename Return, typename... Args>
void *fp_cast(Return (*fp)(Args...))
{
    union {
        Return (*p)(Args...);
        void *v;
    } temp;
    temp.p = fp;
    return temp.v;
}

/**
 * @brief Cast a member function pointer to void pointer.
 *
 * @tparam Return The return type of the member function.
 * @tparam Class The class type that the member function belongs to.
 * @tparam Args The argument types of the member function.
 *
 * @param fp Pointer to the member function to be casted.
 *
 * @return A void pointer to the member function.
 */
template <typename Return, typename Class, typename... Args>
void *fp_cast(Return (Class::*fp)(Args...))
{
    union {
        Return (Class::*p)(Args...);
        void *v;
    } temp;
    temp.p = fp;
    return temp.v;
}

/**
 * @brief Cast a constant member function pointer to void pointer.
 *
 * @tparam Return The return type of the constant member function.
 * @tparam Class The class type that the constant member function belongs to.
 * @tparam Args The argument types of the constant member function.
 *
 * @param fp Pointer to the constant member function to be casted.
 *
 * @return A void pointer to the constant member function.
 */
template <typename Return, typename Class, typename... Args>
void *fp_cast(Return (Class::*fp)(Args...) const)
{
    union {
        Return (Class::*p)(Args...) const;
        void *v;
    } temp;
    temp.p = fp;
    return temp.v;
}

template <typename Return, typename... Arg>
Return (*fp_cast(Return (*fp)(Arg...), void *func))(Arg...)
{
    return *reinterpret_cast<decltype(&fp)>(&func);
}

template <typename Return, typename Class, typename... Arg>
Return (Class::*fp_cast(Return (Class::*fp)(Arg...), void *address))(Arg...)
{
    struct {  // https://doi.org/10.1145/3660779
        void *ptr;
        std::size_t adj = 0;
    } temp;
    temp.ptr = address;
    return *reinterpret_cast<decltype(&fp)>(&temp);
}

/**
 * @brief Gets the original member function pointer from a detour member function pointer (const, no ref-qualifier)
 */
template <typename Return, typename Class, typename... Arg>
Return (Class::*fp_cast(Return (Class::*fp)(Arg...) const, void *address))(Arg...) const
{
    struct {  // https://doi.org/10.1145/3660779
        void *ptr;
        std::size_t adj = 0;
    } temp;
    temp.ptr = address;
    return *reinterpret_cast<decltype(&fp)>(&temp);
}

/**
 * Gets the current Server singleton
 *
 * @return Server instance being run
 */
inline Server &getServer()
{
    static Server *server;
    if (server == nullptr) {
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

        server = &fn();
        if (!server) {
            throw std::runtime_error("Failed to get server instance.");
        }
    }
    return *server;
}
}  // namespace detail
}  // namespace endstone
