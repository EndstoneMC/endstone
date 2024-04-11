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

#include <csignal>

#include <cpptrace/cpptrace.hpp>

namespace endstone::detail {

namespace {

void signal_handler(int signum, siginfo_t *info, void *ctx)
{
    // TODO(crash_dump): add more info here, such as endstone version
    cpptrace::generate_trace(1).print_with_snippets();
    std::quick_exit(1);
}

}  // namespace

void register_signal_handler()
{
    const static std::vector<int> posix_signals = {
        SIGABRT, SIGBUS, SIGFPE, SIGILL, SIGIOT, SIGQUIT, SIGSEGV, SIGSYS, SIGTRAP, SIGXCPU, SIGXFSZ,
    };

    for (int posix_signal : posix_signals) {
        struct sigaction action;
        memset(&action, 0, sizeof action);
        action.sa_flags = static_cast<int>(SA_SIGINFO | SA_NODEFER | SA_RESETHAND);
        sigfillset(&action.sa_mask);
        sigdelset(&action.sa_mask, posix_signal);
        action.sa_sigaction = &signal_handler;
        int r = sigaction(posix_signal, &action, nullptr);
        if (r < 0) {
            printf("WARN: sigaction failed: %d\n", posix_signal);
        }
    }
}

}  // namespace endstone::detail

#endif
