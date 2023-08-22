//
// Created by Vincent on 31/07/2023.
//

#ifdef _WIN32

#include "endstone/common.h"
#include "endstone/endstone.h"
#include "hook_error.h"
#include "hook_manager.h"

#include <DbgHelp.h>
#include <MinHook.h>
#include <Psapi.h>
#include <nlohmann/json.hpp>

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

    MH_STATUS status;
    status = MH_Initialize();
    if (status != MH_OK)
    {
        throw std::system_error(minhook::make_error_code(status));
    }

    registerHooks();

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
    }

    status = MH_EnableHook(MH_ALL_HOOKS);
    if (status != MH_OK)
    {
        throw std::system_error(minhook::make_error_code(status));
    }

    initialized_ = true;
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

void saveSymbolsToCache(const std::map<std::string, size_t> &symbols)
{
    auto path = std::filesystem::current_path() / "config" / "endstone";
    if (!std::filesystem::exists(path))
    {
        std::filesystem::create_directories(path);
    }

    std::ofstream file(path / "symbols.json");
    if (!file.is_open())
    {
        return;
    }

    nlohmann::json json_data;
    json_data["version"] = Endstone::getMinecraftVersion();

    // Save symbols
    for (const auto &[key, value] : symbols)
    {
        json_data["symbols"][key] = value;
    }

    file << json_data;
}

void loadSymbolsFromCache(std::map<std::string, size_t> &symbols)
{
    auto path = std::filesystem::current_path() / "config" / "endstone" / "symbols.json";
    std::ifstream file(path);
    if (!file.is_open())
    {
        return;
    }

    nlohmann::json json_data;
    file >> json_data;

    // Check if the version field matches
    if (json_data["version"] != Endstone::getMinecraftVersion())
    {
        return;
    }

    // Clear existing cache
    symbols.clear();

    // Load symbols
    for (auto &[key, value] : json_data["symbols"].items())
    {
        symbols[key] = value.get<size_t>();
    }
}

void *lookupSymbol(const char *symbol)
{
    static std::map<std::string, size_t> symbol_map;
    static char *base_address = nullptr;
    static std::mutex mtx;
    static bool symbol_loaded = false;

    std::lock_guard<std::mutex> lock(mtx);

    // Attempt to load the cache from a file if it's empty.
    if (symbol_map.empty())
    {
        loadSymbolsFromCache(symbol_map);
    }

    if (!base_address)
    {
        MODULEINFO mi = {nullptr};
        if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandle(nullptr), &mi, sizeof(mi)))
        {
            throw std::system_error(GetLastError(), std::system_category(), "GetModuleInformation failed");
        }

        base_address = static_cast<char *>(mi.lpBaseOfDll);
    }

    // Check if the symbol is in the cache.
    auto it = symbol_map.find(symbol);
    if (it != symbol_map.end())
    {
        auto offset = it->second;
        return base_address + offset;
    }

    // Initialise symbol handler if not valid
    if (!symbol_loaded)
    {
        loadSymbols();
        symbol_loaded = true;
    }

    // https://learn.microsoft.com/en-us/windows/win32/debug/retrieving-symbol-information-by-name
    ULONG64 buffer[(sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR) + sizeof(ULONG64) - 1) / sizeof(ULONG64)];
    auto symbol_info = (PSYMBOL_INFO)buffer;

    symbol_info->SizeOfStruct = sizeof(SYMBOL_INFO);
    symbol_info->MaxNameLen = MAX_SYM_NAME;

    if (!SymFromName(GetCurrentProcess(), symbol, symbol_info))
    {
        throw std::system_error(GetLastError(), std::system_category(), "SymFromName failed");
    }

    auto offset = symbol_info->Address - 0x140000000;

    // Save the symbol to the cache.
    symbol_map[symbol] = offset;

    // Optionally save the cache to a file.
    saveSymbolsToCache(symbol_map);

    return base_address + offset;
}

#endif // _WIN32
