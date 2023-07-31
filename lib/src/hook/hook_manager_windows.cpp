//
// Created by Vincent on 31/07/2023.
//

#ifdef _WIN32

#include "hook_error.h"
#include "hook_manager.h"
#include "server.h"

#include <DbgHelp.h>
#include <MinHook.h>
#include <Psapi.h>

#define RTLD_NEXT GetCurrentProcess()

const IHook &HookManager::getHook(const std::string &symbol)
{
    return hooks[symbol];
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

void *dlsym(void *handle, const char *symbol)
{
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

void HookManager::installHooks()
{
    if (is_initialised)
    {
        return;
    }

    MH_STATUS status;

    printf("Initialising MinHook...\n");
    status = MH_Initialize();
    if (status != MH_OK)
    {
        throw std::system_error(minhook::make_error_code(status));
    }

    printf("Loading symbols...\n");
    loadSymbols();

    printf("Adding hooks...\n");
    HOOK_FUNCTION(ServerInstance::startServerThread)

    printf("Creating hooks...\n");
    for (const auto &item : hooks)
    {
        auto &hook = const_cast<IHook &>(item.second);
        printf("%p -> %p\n", hook.p_target, hook.p_detour);
        status = MH_CreateHook(hook.p_target, hook.p_detour, &hook.p_original);
        if (status != MH_OK)
        {
            throw std::system_error(minhook::make_error_code(status));
        }
    }

    printf("Enabling hooks...\n");
    status = MH_EnableHook(MH_ALL_HOOKS);
    if (status != MH_OK)
    {
        throw std::system_error(minhook::make_error_code(status));
    }

    is_initialised = true;
    printf("Installed all hooks.\n");
}

void HookManager::uninstallHooks()
{
    if (!is_initialised)
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

    hooks.clear();
    is_initialised = false;
}

void ServerInstance::startServerThread()
{
    py::scoped_interpreter interpreter;
    py::gil_scoped_release release;
    printf("Loading plugins...\n");
    auto &server = Server::getInstance();
    server.getPluginManager().load_plugins(std::filesystem::current_path() / "plugins");
    printf("Enabling plugins...\n");
    server.enablePlugins();


    printf("Hook ServerInstance::startServerThread() is called!\n");
    CALL_ORIGINAL(ServerInstance::startServerThread);
    printf("Original ServerInstance::startServerThread() is called!\n");
}

#endif // _WIN32