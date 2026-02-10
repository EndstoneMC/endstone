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

#include <algorithm>
#include <unordered_set>

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif

#ifdef __linux__
#include <csignal>
#endif

#include <sentry.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

#include <cpptrace/cpptrace.hpp>
#include <cpptrace/formatting.hpp>
#include <date/date.h>
#include <fmt/color.h>
#include <fmt/format.h>
#include <fmt/std.h>
#include <spdlog/spdlog.h>

#include "endstone/core/platform.h"
#include "endstone/detail.h"

namespace fs = std::filesystem;

namespace endstone::core {

namespace {

const cpptrace::formatter &get_formatter()
{
    static auto formatter =
        cpptrace::formatter{}  //
            .transform([](cpptrace::stacktrace_frame frame) {
                if (frame.filename.empty()) {
                    frame.filename = frame.get_object_info().object_path;
                }
                return frame;
            })
            .filter([](const cpptrace::stacktrace_frame &frame) {
                auto filename = cpptrace::basename(frame.filename);
                if (filename.starts_with("sentry_") || filename.starts_with("crashpad_")) {
                    return false;
                }
#ifdef _WIN32
                std::ranges::transform(filename, filename.begin(), ::tolower);
                static const std::unordered_set<std::string_view> ignored_modules = {
                    "crash_handler.cpp", "ntdll.dll", "kernel32.dll", "kernelbase.dll", "ucrtbase.dll"};
#elif __linux__
                if (filename.starts_with("libc.so") || filename.starts_with("libc++.so")) {
                    return false;
                }
                static const std::unordered_set<std::string_view> ignored_modules = {"crash_handler.cpp",
                                                                                     "pthread_create.c", "clone3.S"};
#endif
                if (ignored_modules.contains(filename)) {
                    return false;
                }
                return true;
            })
            .filtered_frame_placeholders(false)
            .addresses(cpptrace::formatter::address_mode::object)
            .paths(cpptrace::formatter::path_mode::basename)
            .snippets(true);
    return formatter;
}

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

bool should_print(const sentry_ucontext_t *ctx)
{
#ifdef _WIN32
    const auto *record = ctx->exception_ptrs.ExceptionRecord;
    if (record->ExceptionCode == STATUS_FATAL_APP_EXIT) {
        // Skip printing when the app crashes during intentional shutdown
        // (e.g., via std::exit or ExitProcess).
        return false;
    }
#endif
    return true;
}

void print_crash_message(std::ostream &stream, const sentry_ucontext_t *ctx)
{
    stream << "=== ENDSTONE CRASHED! ===" << '\n'
           << std::left << std::setw(18) << "Platform:" << get_platform() << '\n'
           << std::left << std::setw(18) << "Endstone version:" << ENDSTONE_VERSION << '\n'
           << std::left << std::setw(18) << "Api version:" << ENDSTONE_API_VERSION << '\n';
#ifdef _WIN32
    const auto *record = ctx->exception_ptrs.ExceptionRecord;
    stream << std::left << std::setw(18) << "Exception code:" << "0x" << std::hex << record->ExceptionCode;
    for (const auto &[code, name, description] : EXCEPTION_DEFINITIONS) {
        if (code == record->ExceptionCode) {
            stream << " (" << name << ") - " << description;
        }
    }
#else
    const auto signum = ctx->signum;
    stream << std::left << std::setw(18) << "Signal:" << "0x" << std::hex << signum;
    for (const auto &[signal, name, description] : SIGNAL_DEFINITIONS) {
        if (signal == signum) {
            stream << " (" << name << ") - " << description;
        }
    }
#endif
    stream << "\n";
}

std::string get_filename_formatted_date_time()
{
    const auto now = std::chrono::system_clock::now();
    const auto tp = date::floor<std::chrono::seconds>(now);
    std::ostringstream oss;
    oss << date::format("%Y-%m-%d-%H.%M.%S", tp);
    return oss.str();
}

sentry_value_t on_crash(const sentry_ucontext_t *ctx, const sentry_value_t event, void * /*closure*/)
{
    if (!should_print(ctx)) {
        sentry_value_decref(event);
        return sentry_value_new_null();
    }

    const auto stacktrace = cpptrace::generate_trace();
    print_crash_message(std::cerr, ctx);
    print_stacktrace(std::cerr, stacktrace);

    try {
        fs::path path = "./crash_reports/";
        fs::create_directories(path);
        path = path / fmt::format("crash-{}-server.txt", get_filename_formatted_date_time());
        std::ofstream out(path, std::ios::out | std::ios::trunc);
        print_crash_message(out, ctx);
        print_stacktrace(out, stacktrace);
        fmt::print(fmt::fg(fmt::terminal_color::red), "This crash report has been saved to: {}\n", absolute(path));
    }
    catch (...) {
        fmt::print(fmt::fg(fmt::terminal_color::red), "We were unable to save this crash report to disk.\n");
    }
    return event;
}
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
}

CrashHandler::~CrashHandler()
{
    sentry_close();
}

void print_stacktrace(std::ostream &stream, const cpptrace::stacktrace &stacktrace)
{
    get_formatter().print(stream, stacktrace);
}

}  // namespace endstone::core
