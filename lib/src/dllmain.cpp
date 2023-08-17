#ifdef _WIN32

#include <Windows.h>

#include "common.h"
#include "endstone/endstone.h"
#include "endstone/server.h"
#include "hook/hook_manager.h"

BOOL WINAPI DllMain(_In_ HINSTANCE,          // handle to DLL module
                    _In_ DWORD fdwReason,    // reason for calling function
                    _In_ LPVOID lpvReserved) // reserved
{
    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH: {
        try
        {
            // Initialize once for each new process.
            HookManager::initialize();
            Endstone::setServer(std::make_unique<Server>());
            break;
        }
        catch (const std::exception &e)
        {
            printf("LibEndstone loads failed.\n");
            if (const auto *se = dynamic_cast<const std::system_error *>(&e))
            {
                printf("%s, error code: %d.\n", se->what(), se->code().value());
            }
            else
            {
                printf("%s\n", e.what());
            }
            return FALSE; // Return FALSE to fail DLL load.
        }
    }
    case DLL_PROCESS_DETACH: {
        if (lpvReserved != nullptr)
        {
            break; // do not do cleanup if process termination scenario
        }

        // Perform any necessary cleanup.
        HookManager::finalize();

        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    default:
        // Do thread-specific initialization or cleanup.
        break;
    }

    return TRUE; // Successful DLL_PROCESS_ATTACH.
}

#endif // _WIN32