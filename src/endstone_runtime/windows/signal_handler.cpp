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

#include <csignal>
#include <iostream>

#include <cpptrace/cpptrace.hpp>

#include "Windows.h"

namespace endstone::detail {

namespace {

void print_crash_dump()
{
    // TODO(crash_dump): add more info here, such as endstone version
    cpptrace::generate_trace(2).print_with_snippets();
}

void terminate_handler()
{
    print_crash_dump();
    std::quick_exit(1);
}

void purecall_handler()
{
    printf("Pure virtual function called!\n");
    print_crash_dump();
    std::quick_exit(1);
}

void signal_handler(int)
{
    print_crash_dump();
    std::quick_exit(1);
}

LONG WINAPI exception_filter(EXCEPTION_POINTERS *info)
{
    print_crash_dump();
    return EXCEPTION_CONTINUE_SEARCH;
}

}  // namespace

void register_signal_handler()
{
    std::set_terminate(terminate_handler);
    _set_purecall_handler(purecall_handler);
    signal(SIGABRT, signal_handler);
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
    SetUnhandledExceptionFilter(exception_filter);
}

}  // namespace endstone::detail

#endif
