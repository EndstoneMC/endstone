#ifdef _WIN32

#include <Windows.h>

#include "endstone.h"
#include "hook_manager.h"
#include "server.h"

BOOL WINAPI DllMain(_In_ HINSTANCE,          // handle to DLL module
                    _In_ DWORD fdwReason,    // reason for calling function
                    _In_ LPVOID lpvReserved) // reserved
{
    static PyThreadState *state = nullptr;

    // Perform actions based on the reason for calling.
    switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH: {
        try
        {
            // Initialize once for each new process.

            // Initialize Python interpreter
            py::initialize_interpreter();
            // Release the GIL from main thread so that it can acquired by the server thread
            state = PyEval_SaveThread();

            {
                // Destroy the thread state
                // Reference: https://github.com/wjakob/nanogui/blob/master/python/main.cpp#L89
                py::gil_scoped_acquire acquire{};
                acquire.dec_ref();
            }

            HookManager::initialize();
            break;
        }
        catch (const std::system_error &e)
        {
            printf("LibEndStone loads failed.\n");
            printf("%s, error code: %d.\n", e.what(), e.code().value());
            return FALSE; // Return FALSE to fail DLL load.
        }
    }
    case DLL_PROCESS_DETACH: {
        // Reacquire GIL and associate with thread state
        PyEval_RestoreThread(state);
        py::finalize_interpreter();

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