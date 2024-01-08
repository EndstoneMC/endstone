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

#include "endstone_runtime/symbol_handler.h"

#include <Windows.h>
// must be included after Windows.h
#include <DbgHelp.h>

namespace endstone::sym {

namespace {
class WindowsSymbolHandler : public SymbolHandler {
public:
    explicit WindowsSymbolHandler(int sym_options = 0, const char *search_path = nullptr, bool invade_process = false)
    {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<size_t> dist(1, static_cast<size_t>(-1));
        handle_ = reinterpret_cast<HANDLE>(dist(gen));  // NOLINT(*-no-int-to-ptr)

        SymSetOptions(sym_options);

        if (!SymInitialize(handle_, search_path, invade_process ? TRUE : FALSE)) {
            throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "SymInitialize failed");
        }
    }

    ~WindowsSymbolHandler() override
    {
        if (valid_) {
            std::lock_guard lock{mutex_};
            SymCleanup(handle_);
        }
    }

    void enumerate(const char *module, std::function<bool(const std::string &, size_t)> callback) override
    {
        std::lock_guard lock{mutex_};

        auto module_base = SymLoadModuleEx(handle_, nullptr, module, nullptr, 0, 0, nullptr, 0);
        if (!module_base) {
            throw std::system_error(static_cast<int>(GetLastError()), std::system_category(), "SymLoadModuleEx failed");
        }

        struct UserContext {
            size_t module_base;
            std::function<bool(const std::string &, size_t)> callback;
        };
        auto user_context = UserContext{module_base, std::move(callback)};

        SymEnumSymbols(
            handle_, module_base, "*" /*Mask*/,
            [](PSYMBOL_INFO info /*pSymInfo*/, ULONG /*SymbolSize*/, PVOID user_context /*UserContext*/) -> BOOL {
                auto *ctx = static_cast<UserContext *>(user_context);
                auto should_continue = (ctx->callback)(info->Name, info->Address - ctx->module_base);
                return should_continue ? TRUE : FALSE;
            },
            &user_context);
    }

private:
    HANDLE handle_;
    std::mutex mutex_;
};
}  // namespace

std::unique_ptr<SymbolHandler> create_handler()
{
    return std::make_unique<WindowsSymbolHandler>();
}
}  // namespace endstone::sym

#endif  // _WIN32
