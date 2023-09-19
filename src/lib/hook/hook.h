// Copyright (c) 2023, The Endstone Project. (https://endstone.dev) All Rights Reserved.
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

#include <string>
#include <utility>
#include <vector>

#include "lib/hook/internal.h"

namespace Endstone::Hook {

template <typename ReturnType = void *>
inline ReturnType sym_from_name(const std::string &name)
{
    auto &internals = Endstone::HookInternal::get_internals();
    auto it = internals.originals.find(name);
    if (it == internals.originals.end()) {
        throw std::runtime_error("Symbol " + name + " not found.");
    }

    return reinterpret_cast<ReturnType>(it->second);
}

/**
 * @brief Construct a std::function from a vanilla function pointer
 */
template <typename Return, typename... Arg>
inline std::function<Return(Arg...)> get_function(Return (*)(Arg...), const std::string &name)
{
    return reinterpret_cast<Return (*)(Arg...)>(sym_from_name(name));
}

/**
 * @brief Construct a std::function from a class method (non-const, no ref-qualifier)
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return(Class *, Arg...)> get_function(Return (Class::*)(Arg...), const std::string &name)
{
    auto func = reinterpret_cast<Return (*)(Class *, Arg...)>(sym_from_name(name));
    return [func](Class *obj, Arg... args) -> Return {
        return func(obj, std::forward<Arg>(args)...);
    };
}

/**
 * @brief Construct a std::function from a class method (const, no ref-qualifier)
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return(const Class *, Arg...)> get_function(Return (Class::*)(Arg...) const,
                                                                 const std::string &name)
{
    auto func = reinterpret_cast<Return (*)(const Class *, Arg...)>(sym_from_name(name));
    return [func](const Class *obj, Arg... args) -> Return {
        return func(obj, std::forward<Arg>(args)...);
    };
}

/**
 * @brief Construct a std::function from a class method (non-const, no ref-qualifier)
 *  with considerations for Return Value Optimization (RVO).
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return *(Class *, Return *, Arg...)> get_function_rvo(Return (Class::*)(Arg...),
                                                                           const std::string &name)
{
    auto func = reinterpret_cast<Return *(*)(Class *, Return *, Arg...)>(sym_from_name(name));
    return [func](Class *obj, Return *ret, Arg... args) -> Return * {
        return func(obj, ret, std::forward<Arg>(args)...);
    };
}

/**
 * @brief Construct a std::function from a class method (const, no ref-qualifier)
 *  with considerations for Return Value Optimization (RVO).
 */
template <typename Return, typename Class, typename... Arg>
inline std::function<Return *(const Class *, Return *, Arg...)> get_function_rvo(Return (Class::*)(Arg...) const,
                                                                                 const std::string &name)
{
    auto func = reinterpret_cast<Return *(*)(const Class *, Return *, Arg...)>(sym_from_name(name));
    return [func](const Class *obj, Return *ret, Arg... args) -> Return * {
        return func(obj, ret, std::forward<Arg>(args)...);
    };
}

}  // namespace Endstone::Hook

#ifdef _WIN32
#include <MinHook.h>

namespace Endstone::Hook {
class HookManager {
public:
    explicit HookManager(void *h_library)
    {
        Endstone::HookInternal::SymbolHandler sym{0, nullptr, false};
        auto &internals = Endstone::HookInternal::get_internals();

        void *module_base;
        size_t image_base;

        module_base = Endstone::HookInternal::get_module_base(GetCurrentProcess(), h_library);
        image_base = sym.load_module(h_library);
        sym.enum_symbols(image_base, "*", [&](auto info, auto /*size*/) -> bool {
            internals.detours.insert(
                {std::string(info->Name), static_cast<char *>(module_base) + (info->Address - image_base)});
            return true;
        });

        const static auto SpinnerChars = std::vector<std::string>{"⠈", "⠐", "⠠", "⢀", "⡀", "⠄", "⠂", "⠁"};
        size_t spinner_id = 0;
        auto last_time = std::chrono::steady_clock::now();

        module_base = Endstone::HookInternal::get_module_base(GetCurrentProcess(), GetModuleHandle(nullptr));
        image_base = sym.load_module(nullptr);
        sym.enum_symbols(image_base, "*", [&](auto info, auto /*size*/) -> bool {
            auto name = std::string(info->Name);
            if (internals.detours.find(name) == internals.detours.end()) {
                // Not used for hooking; we proceed to undecorate it for easier lookup and to save some memory.
                char buffer[4096];
                auto len = UnDecorateSymbolName(name.c_str(), buffer, sizeof(buffer), UNDNAME_NAME_ONLY);

                if (!len) {
                    throw std::system_error(static_cast<int>(GetLastError()), std::system_category(),
                                            "UnDecorateSymbolName failed");
                }
                name = std::string(buffer, len);
            }

            internals.originals.insert({name, static_cast<char *>(module_base) + (info->Address - image_base)});

            // Update the progress indicator
            using std::chrono_literals::operator""ms;
            auto now = std::chrono::steady_clock::now();
            if ((now - last_time) >= 40ms) {
                printf("\r\x1b[93;1m%s Loading endstone...", SpinnerChars[spinner_id % SpinnerChars.size()].c_str());
                fflush(stdout);
                spinner_id++;
                last_time = now;
            }

            return true;
        });

        printf("\r\x1b[0m");  // Dismiss the progress indicator

        MH_STATUS status;
        status = MH_Initialize();
        if (status != MH_OK) {
            throw std::system_error(status, Endstone::HookInternal::minhook_category());
        }

        for (const auto &[name, detour] : internals.detours) {
            auto *target = sym_from_name(name);
            void *original = nullptr;

            // printf("%s: 0x%p -> 0x%p\n", name.c_str(), target, detour);
            status = MH_CreateHook(target, detour, &original);
            if (status != MH_OK) {
                throw std::system_error(status, Endstone::HookInternal::minhook_category());
            }

            // printf("%s: = 0x%p\n", name.c_str(), original);
            internals.originals[name] = original;
        }

        status = MH_EnableHook(MH_ALL_HOOKS);
        if (status != MH_OK) {
            throw std::system_error(status, Endstone::HookInternal::minhook_category());
        }

        initialized_ = true;
    }

    HookManager(const HookManager &) = delete;
    HookManager(HookManager &&other) noexcept
    {
        other.initialized_ = false;
    }
    HookManager &operator=(const HookManager &) = delete;
    HookManager &operator=(HookManager &&) = delete;

    ~HookManager()
    {
        if (initialized_) {
            MH_DisableHook(MH_ALL_HOOKS);
            MH_Uninitialize();
        }
    }

private:
    bool initialized_ = false;
};
}  // namespace Endstone::Hook

#endif

#define ENDSTONE_HOOK_CALL_ORIGINAL(fp, ...)     Endstone::Hook::get_function(fp, __FUNCDNAME__)(__VA_ARGS__);
#define ENDSTONE_HOOK_CALL_ORIGINAL_RVO(fp, ...) Endstone::Hook::get_function_rvo(fp, __FUNCDNAME__)(__VA_ARGS__);
