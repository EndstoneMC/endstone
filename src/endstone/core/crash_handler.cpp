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

#include "endstone/core/crash_handler.h"

#include <sentry.h>

#include <csignal>
#include <filesystem>
#include <iostream>
#include <string>

#include <cpptrace/cpptrace.hpp>
#include <fmt/format.h>

#include "endstone/detail/common.h"
#include "endstone/detail/platform.h"

#ifdef _WIN32
#include <Windows.h>
#endif

namespace fs = std::filesystem;

namespace endstone::core {

namespace {

#ifdef _WIN32
struct exception_slot {
    DWORD code;
    const char *name;
    const char *description;
};

#define EXCEPTION_DEF(code, desc) {code, #code, desc}

const exception_slot EXCEPTION_DEFINITIONS[] = {
    EXCEPTION_DEF(EXCEPTION_ACCESS_VIOLATION, "AccessViolation"),
    EXCEPTION_DEF(EXCEPTION_ARRAY_BOUNDS_EXCEEDED, "ArrayBoundsExceeded"),
    EXCEPTION_DEF(EXCEPTION_BREAKPOINT, "BreakPoint"),
    EXCEPTION_DEF(EXCEPTION_DATATYPE_MISALIGNMENT, "DatatypeMisalignment"),
    EXCEPTION_DEF(EXCEPTION_FLT_DENORMAL_OPERAND, "FloatDenormalOperand"),
    EXCEPTION_DEF(EXCEPTION_FLT_DIVIDE_BY_ZERO, "FloatDivideByZero"),
    EXCEPTION_DEF(EXCEPTION_FLT_INEXACT_RESULT, "FloatInexactResult"),
    EXCEPTION_DEF(EXCEPTION_FLT_INVALID_OPERATION, "FloatInvalidOperation"),
    EXCEPTION_DEF(EXCEPTION_FLT_OVERFLOW, "FloatOverflow"),
    EXCEPTION_DEF(EXCEPTION_FLT_STACK_CHECK, "FloatStackCheck"),
    EXCEPTION_DEF(EXCEPTION_FLT_UNDERFLOW, "FloatUnderflow"),
    EXCEPTION_DEF(EXCEPTION_ILLEGAL_INSTRUCTION, "IllegalInstruction"),
    EXCEPTION_DEF(EXCEPTION_IN_PAGE_ERROR, "InPageError"),
    EXCEPTION_DEF(EXCEPTION_INT_DIVIDE_BY_ZERO, "IntegerDivideByZero"),
    EXCEPTION_DEF(EXCEPTION_INT_OVERFLOW, "IntegerOverflow"),
    EXCEPTION_DEF(EXCEPTION_INVALID_DISPOSITION, "InvalidDisposition"),
    EXCEPTION_DEF(EXCEPTION_NONCONTINUABLE_EXCEPTION, "NonContinuableException"),
    EXCEPTION_DEF(EXCEPTION_PRIV_INSTRUCTION, "PrivilgedInstruction"),
    EXCEPTION_DEF(EXCEPTION_SINGLE_STEP, "SingleStep"),
    EXCEPTION_DEF(EXCEPTION_STACK_OVERFLOW, "StackOverflow")};

#endif

#ifdef __linux__
struct signal_slot {
    int signal;
    const char *name;
    const char *description;
};

#define SIGNAL_DEF(sig, desc) {sig, #sig, desc}

const signal_slot SIGNAL_DEFINITIONS[] = {SIGNAL_DEF(SIGILL, "IllegalInstruction"),
                                          SIGNAL_DEF(SIGTRAP, "Trap"),
                                          SIGNAL_DEF(SIGABRT, "Abort"),
                                          SIGNAL_DEF(SIGBUS, "BusError"),
                                          SIGNAL_DEF(SIGFPE, "FloatingPointException"),
                                          SIGNAL_DEF(SIGSEGV, "Segfault")};
#endif

void print_frame(std::ostream &stream, bool color, unsigned frame_number_width, std::size_t counter,
                 const cpptrace::stacktrace_frame &frame)
{
    const auto *reset = color ? "\033[0m" : "";
    const auto *green = color ? "\033[32m" : "";
    const auto *yellow = color ? "\033[33m" : "";
    const auto *blue = color ? "\033[34m" : "";
    std::string line = fmt::format("[{:<{}}] ", counter, frame_number_width);
    if (frame.is_inline) {
        line += fmt::format("{:<{}}", "(inlined)", 2 * sizeof(cpptrace::frame_ptr) + 2);
    }
    else {
        line += fmt::format("{}0x{:<{}x}{}", blue, frame.raw_address, 2 * sizeof(cpptrace::frame_ptr), reset);
        line += fmt::format(" {}(0x{:09x}){}", green, frame.object_address, reset);
    }
    if (!frame.symbol.empty()) {
        line += fmt::format(" in {}{}{}", yellow, frame.symbol, reset);
    }
    if (!frame.filename.empty()) {
        line += fmt::format(" at {}{}{}", green, frame.filename, reset);
        if (frame.line.has_value()) {
            line += fmt::format(":{}{}{}", blue, frame.line.value(), reset);
            if (frame.column.has_value()) {
                line += fmt::format(":{}{}{}", blue, frame.column.value(), reset);
            }
        }
    }
    stream << line;
}

bool should_report(const sentry_ucontext_t *ctx)
{
#ifdef _WIN32
    const auto *record = ctx->exception_ptrs.ExceptionRecord;
    if (record->ExceptionCode == STATUS_FATAL_APP_EXIT) {
        return false;
    }
#endif
    return true;
}

void print_crash_message(std::string_view message)
{
    const auto stacktrace = cpptrace::generate_trace(1);
    auto &stream = std::cerr;
    stream << "=== ENDSTONE CRASHED! ===" << '\n'
           << std::left << std::setw(18) << "Platform:" << detail::get_platform() << '\n'
           << std::left << std::setw(18) << "Endstone version:" << ENDSTONE_VERSION << '\n'
           << std::left << std::setw(18) << "Api version:" << ENDSTONE_API_VERSION << '\n';
    stream << message << "\n";
    stream << "Stack trace (most recent call first):" << '\n';
    if (const auto &frames = stacktrace.frames; frames.empty()) {
        stream << "<empty trace>" << '\n';
    }
    else {
        std::size_t counter = 0;
        const auto color = cpptrace::isatty(cpptrace::stderr_fileno);
        const auto frame_number_width = std::to_string(frames.size()).length();
        for (const auto &frame : frames) {
            print_frame(stream, color, frame_number_width, counter, frame);
            stream << '\n';
            if (frame.line.has_value() && !frame.filename.empty()) {
                stream << cpptrace::get_snippet(frame.filename, frame.line.value(), 2, color);
            }
            counter++;
        }
    }
}

sentry_value_t on_crash(const sentry_ucontext_t *ctx, const sentry_value_t event, void * /*closure*/)
{
    std::stringstream ss;
#ifdef _WIN32
    const auto *record = ctx->exception_ptrs.ExceptionRecord;
    ss << std::left << std::setw(18) << "Exception code:" << "0x" << std::hex << record->ExceptionCode;
    for (const auto &[code, name, description] : EXCEPTION_DEFINITIONS) {
        if (code == record->ExceptionCode) {
            ss << " (" << name << ") - " << description;
        }
    }
#else
    const auto signum = ctx->signum;
    ss << std::left << std::setw(18) << "Signal:" << "0x" << std::hex << signum;
    for (const auto &[signal, name, description] : SIGNAL_DEFINITIONS) {
        if (signal == signum) {
            ss << " (" << name << ") - " << description;
        }
    }
#endif
    print_crash_message(ss.str());

    if (!should_report(ctx)) {
        sentry_value_decref(event);
        return sentry_value_new_null();
    }
    return event;
}

#ifdef _WIN32
void on_terminate()
{
    print_crash_message("Program terminated.");
    std::quick_exit(1);
}

void on_purecall()
{
    print_crash_message("Pure virtual function called!");
    std::quick_exit(1);
}

void on_signal(int signal)
{
    print_crash_message("Signal received: " + std::to_string(signal));
    std::quick_exit(1);
}
#endif

}  // namespace

CrashHandler::CrashHandler()
{
    constexpr auto dsn =
        "https://69c28eeaef4651abcf0bbeace6a1175c@o4508553519431680.ingest.de.sentry.io/4508569040519248";
    constexpr std::string_view release = "endstone@" ENDSTONE_VERSION;
    constexpr bool is_dev = release.find("dev") != std::string_view::npos;

    sentry_options_t *options = sentry_options_new();
    sentry_options_set_dsn(options, dsn);
    sentry_options_set_database_path(options, ".sentry-native");
    sentry_options_set_release(options, std::string(release).c_str());
    sentry_options_set_on_crash(options, on_crash, nullptr);
    sentry_options_set_environment(options, is_dev ? "development" : "production");
    sentry_init(options);

#ifdef _WIN32
    std::set_terminate(on_terminate);
    _set_purecall_handler(on_purecall);
    signal(SIGABRT, on_signal);
    _set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
#endif
}

CrashHandler::~CrashHandler()
{
    sentry_close();
}

}  // namespace endstone::core
