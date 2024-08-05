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

#ifdef _MSC_VER

#include "endstone/detail/signal_handler.h"

#include <csignal>

#include <entt/entt.hpp>
#include <fmt/format.h>

#include "endstone/detail/server.h"

// Must be included after all headers
#include "Windows.h"

namespace endstone::detail {

namespace {

void terminate_handler()
{
    print_crash_dump("Program terminated.");
    std::quick_exit(1);
}

void purecall_handler()
{
    print_crash_dump("Pure virtual function called!");
    std::quick_exit(1);
}

void signal_handler(int signal)
{
    print_crash_dump("Signal received: " + std::to_string(signal));
    std::quick_exit(1);
}

LONG WINAPI exception_filter(EXCEPTION_POINTERS *info)
{
    print_crash_dump("Exception unhandled: " + fmt::format("{0:#x}", info->ExceptionRecord->ExceptionCode));
    return EXCEPTION_CONTINUE_SEARCH;
}

BOOL WINAPI console_ctrl_handler(DWORD event)
{
    switch (event) {
    case CTRL_C_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_SHUTDOWN_EVENT:
    case SIGTERM: {
        auto &server = entt::locator<EndstoneServer>::value();
        server.shutdown();
        return TRUE;
    }
    default:
        return FALSE;
    }
}

void console_signal_handler(int signal)
{
    switch (signal) {
    case SIGINT:
    case SIGTERM: {
        auto &server = entt::locator<EndstoneServer>::value();
        server.shutdown();
    }
    default:
        break;
    }
}

}  // namespace

void register_signal_handler()
{
    std::set_terminate(terminate_handler);
    _set_purecall_handler(purecall_handler);
    signal(SIGABRT, signal_handler);
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
    SetUnhandledExceptionFilter(exception_filter);
    SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
    signal(SIGINT, console_signal_handler);
    signal(SIGTERM, console_signal_handler);
}

}  // namespace endstone::detail

#endif
