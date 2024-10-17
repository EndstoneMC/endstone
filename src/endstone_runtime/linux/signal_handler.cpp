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

#ifdef __linux__

#include "endstone/detail/signal_handler.h"

#include <csignal>

#include <cpptrace/cpptrace.hpp>
#include <entt/entt.hpp>
#include <fmt/format.h>

#include "endstone/detail/server.h"

namespace endstone::detail {

namespace {

void signal_handler(int signum, siginfo_t *info, void *ctx)
{
    print_crash_dump("Signal received: " + std::to_string(signum));
    std::quick_exit(1);
}

void console_signal_handler(int signum, siginfo_t *info, void *ctx)
{
    if (entt::locator<EndstoneServer>::has_value()) {
        auto &server = entt::locator<EndstoneServer>::value();
        server.shutdown();
    }
}

void register_handler(int signal, void (*handler)(int, siginfo_t *, void *))
{
    struct sigaction action;
    memset(&action, 0, sizeof action);
    action.sa_flags = static_cast<int>(SA_SIGINFO | SA_NODEFER | SA_RESETHAND);
    sigfillset(&action.sa_mask);
    sigdelset(&action.sa_mask, signal);
    action.sa_sigaction = handler;
    int r = sigaction(signal, &action, nullptr);
    if (r < 0) {
        printf("WARN: sigaction failed: %d\n", signal);
    }
}

}  // namespace

void register_signal_handler()
{
    register_handler(SIGABRT, &signal_handler);
    register_handler(SIGBUS, &signal_handler);
    register_handler(SIGFPE, &signal_handler);
    register_handler(SIGILL, &signal_handler);
    register_handler(SIGIOT, &signal_handler);
    register_handler(SIGQUIT, &signal_handler);
    register_handler(SIGSEGV, &signal_handler);
    register_handler(SIGSYS, &signal_handler);
    register_handler(SIGTRAP, &signal_handler);
    register_handler(SIGXCPU, &signal_handler);
    register_handler(SIGXFSZ, &signal_handler);

    register_handler(SIGINT, &console_signal_handler);
    register_handler(SIGTERM, &console_signal_handler);
}

}  // namespace endstone::detail

#endif
