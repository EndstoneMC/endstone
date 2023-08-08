//
// Created by Vincent on 31/07/2023.
//

#ifdef _WIN32

#include "endstone.h"
#include "hook/hook_error.h"
#include "hook/hook_manager.h"

#include <DbgHelp.h>
#include <MinHook.h>
#include <Psapi.h>
#include <indicators/progress_spinner.hpp>

const Hook &HookManager::getHook(const std::string &symbol)
{
    return hooks_[symbol];
}

void loadSymbols()
{
    auto handle = GetCurrentProcess();

    // Initialise sym handler
    SymSetOptions(SYMOPT_UNDNAME); // Undecorate function names
    if (!SymInitialize(handle, nullptr, false))
    {
        throw std::system_error(GetLastError(), std::system_category(), "SymInitialize failed");
    }

    WCHAR image_name[MAX_PATH];
    auto len = GetModuleFileNameEx(handle, nullptr, image_name, MAX_PATH);
    if (len == 0 || len == MAX_PATH)
    {
        throw std::system_error(GetLastError(), std::system_category(), "GetModuleFileNameEx failed");
    }

    if (!SymLoadModuleExW(handle, nullptr, image_name, nullptr, 0, 0, nullptr, 0))
    {
        throw std::system_error(GetLastError(), std::system_category(), "SymLoadModuleExW failed");
    }
}

void HookManager::initialize()
{
    if (initialized_)
    {
        return;
    }

    using namespace indicators;
    indicators::ProgressSpinner spinner{option::PostfixText{"EndStone is now loading..."},
                                        option::ForegroundColor{Color::yellow},
                                        option::SpinnerStates{std::vector<std::string>{"-", "\\", "|", "/"}},
                                        option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}};
    std::this_thread::sleep_for(std::chrono::milliseconds(40));

    MH_STATUS status;

    spinner.set_option(option::PostfixText{"Initialising MinHook..."});
    status = MH_Initialize();
    if (status != MH_OK)
    {
        throw std::system_error(minhook::make_error_code(status));
    }
    spinner.set_progress(5);

    spinner.set_option(option::PostfixText{"Loading symbols..."});
    loadSymbols();
    spinner.set_progress(10);

    registerHooks();

    spinner.set_option(option::PostfixText{"Creating hooks..."});
    auto step = (90 - spinner.current()) / hooks_.size();
    for (const auto &item : hooks_)
    {
        auto symbol = item.first.c_str();
        auto &hook = const_cast<Hook &>(item.second);
        hook.p_target = lookupSymbol(symbol);
        // printf("%p -> %p\n", hook.p_target, hook.p_detour);
        status = MH_CreateHook(hook.p_target, hook.p_detour, &hook.p_original);
        if (status != MH_OK)
        {
            throw std::system_error(minhook::make_error_code(status));
        }
        spinner.set_progress(spinner.current() + step);
    }

    spinner.set_option(option::PostfixText{"Enabling hooks..."});
    status = MH_EnableHook(MH_ALL_HOOKS);
    if (status != MH_OK)
    {
        throw std::system_error(minhook::make_error_code(status));
    }
    spinner.set_progress(95);

    initialized_ = true;
    spinner.set_option(option::ForegroundColor{Color::green});
    //    spinner.set_option(option::PrefixText{""});
    spinner.set_option(option::ShowSpinner{false});
    spinner.set_option(option::ShowPercentage{false});
    spinner.set_option(option::PostfixText{"EndStone loaded successfully!"});
    spinner.mark_as_completed();
}

void HookManager::finalize()
{
    if (!initialized_)
    {
        return;
    }

    MH_STATUS status;

    status = MH_DisableHook(MH_ALL_HOOKS);
    if (status != MH_OK)
    {
        throw std::system_error(minhook::make_error_code(status));
    }

    status = MH_Uninitialize();
    if (status != MH_OK)
    {
        throw std::system_error(minhook::make_error_code(status));
    }

    hooks_.clear();
    initialized_ = false;
}

void *lookupSymbol(const char *symbol)
{
    auto handle = GetCurrentProcess();
    MODULEINFO mi = {nullptr};
    if (!GetModuleInformation(handle, GetModuleHandle(nullptr), &mi, sizeof(mi)))
    {
        throw std::system_error(GetLastError(), std::system_category(), "GetModuleInformation failed");
    }

    // https://learn.microsoft.com/en-us/windows/win32/debug/retrieving-symbol-information-by-name
    ULONG64 buffer[(sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR) + sizeof(ULONG64) - 1) / sizeof(ULONG64)];
    auto symbol_info = (PSYMBOL_INFO)buffer;

    symbol_info->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol_info->MaxNameLen = MAX_SYM_NAME;

    if (!SymFromName(handle, symbol, symbol_info))
    {
        throw std::system_error(GetLastError(), std::system_category(), "SymFromName failed");
    }

    return static_cast<char *>(mi.lpBaseOfDll) + symbol_info->Address - 0x140000000;
}

#endif // _WIN32
