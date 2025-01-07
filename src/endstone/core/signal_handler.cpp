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

#include "endstone/core/signal_handler.h"

#include <csignal>

#include <entt/entt.hpp>

#include "endstone/core/server.h"

#ifdef _WIN32
#include <Windows.h>
#endif

namespace endstone::core {
namespace {
void request_server_shutdown()
{
    if (entt::locator<EndstoneServer>::has_value()) {
        auto &server = entt::locator<EndstoneServer>::value();
        server.shutdown();
    }
}

#ifdef _WIN32
BOOL WINAPI console_ctrl_handler(DWORD event)
{
    switch (event) {
    case CTRL_C_EVENT:
    case CTRL_CLOSE_EVENT:
    case CTRL_SHUTDOWN_EVENT:
    case SIGTERM: {
        request_server_shutdown();
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
        request_server_shutdown();
        break;
    }
    default:
        break;
    }
}
#endif
#ifdef __linux__
void console_signal_handler(int signum, siginfo_t *info, void *ctx)
{
    request_server_shutdown();
}
#endif
}  // namespace

SignalHandler::SignalHandler()
{
#ifdef _WIN32
    SetConsoleCtrlHandler(console_ctrl_handler, TRUE);
    signal(SIGINT, console_signal_handler);
    signal(SIGTERM, console_signal_handler);
#endif
#ifdef __linux__
    struct sigaction action;
    memset(&action, 0, sizeof action);
    action.sa_flags = static_cast<int>(SA_SIGINFO | SA_NODEFER | SA_RESETHAND);
    sigfillset(&action.sa_mask);
    action.sa_sigaction = console_signal_handler;
    sigaction(SIGINT, &action, nullptr);
    sigaction(SIGTERM, &action, nullptr);
#endif
}

SignalHandler::~SignalHandler()
{
#ifdef _WIN32
    SetConsoleCtrlHandler(nullptr, FALSE);
    signal(SIGINT, SIG_DFL);
    signal(SIGTERM, SIG_DFL);
#endif
#ifdef __linux__
    struct sigaction action;
    memset(&action, 0, sizeof action);
    action.sa_handler = SIG_DFL;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0;
    sigaction(SIGINT, &action, nullptr);
    sigaction(SIGTERM, &action, nullptr);
#endif
}

}  // namespace endstone::core
