import ctypes
import os
import subprocess
from ctypes import get_last_error
from ctypes.wintypes import (
    BOOL,
    DWORD,
    HANDLE,
    HMODULE,
    LPCSTR,
    LPCVOID,
    LPCWSTR,
    LPDWORD,
    LPVOID,
)

from endstone._internal.bootstrap.base import Bootstrap

SIZE_T = ctypes.c_size_t
PSIZE_T = ctypes.POINTER(SIZE_T)


class THREADENTRY32(ctypes.Structure):
    _fields_ = [
        ("dwSize", DWORD),
        ("cntUsage", DWORD),
        ("th32ThreadID", DWORD),
        ("th32OwnerProcessID", DWORD),
        ("tpBasePri", DWORD),
        ("tpDeltaPri", DWORD),
        ("dwFlags", DWORD),
    ]


# Kernel32 Functions
kernel32 = ctypes.WinDLL("kernel32", use_last_error=True)

# Constants
CREATE_SUSPENDED = 0x00000004  # https://learn.microsoft.com/en-us/windows/win32/procthread/process-creation-flags
MAX_PATH = 260  # https://learn.microsoft.com/en-us/windows/win32/fileio/maximum-file-path-limitation
MEM_COMMIT = 0x00001000  # https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc
MEM_RESERVE = 0x00002000  # https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc
PAGE_READWRITE = 0x04  # https://learn.microsoft.com/en-us/windows/win32/memory/memory-protection-constants
INFINITE = 0xFFFFFFFF
TH32CS_SNAPTHREAD = 0x00000004
THREAD_ALL_ACCESS = 0x000F0000 | 0x00100000 | 0xFFFF
INVALID_HANDLE_VALUE = ctypes.c_void_p(-1).value
DONT_RESOLVE_DLL_REFERENCES = 0x00000001

# https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualallocex
VirtualAllocEx = kernel32.VirtualAllocEx
VirtualAllocEx.restype = LPVOID
VirtualAllocEx.argtypes = (HANDLE, LPVOID, SIZE_T, DWORD, DWORD)

# https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-writeprocessmemory
WriteProcessMemory = kernel32.WriteProcessMemory
WriteProcessMemory.restype = BOOL
WriteProcessMemory.argtypes = (HANDLE, LPVOID, LPCVOID, SIZE_T, PSIZE_T)

# https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createremotethread
CreateRemoteThread = kernel32.CreateRemoteThread
CreateRemoteThread.restype = HANDLE
CreateRemoteThread.argtypes = (HANDLE, LPVOID, SIZE_T, LPVOID, LPVOID, DWORD, LPDWORD)

# https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-getexitcodethread
GetExitCodeThread = kernel32.GetExitCodeThread
GetExitCodeThread.restype = BOOL
GetExitCodeThread.argtypes = (HANDLE, LPDWORD)

# https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulehandlew
GetModuleHandle = kernel32.GetModuleHandleW
GetModuleHandle.restype = HMODULE
GetModuleHandle.argtypes = (LPCWSTR,)

# https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getprocaddress
GetProcAddress = kernel32.GetProcAddress
GetProcAddress.restype = LPVOID
GetProcAddress.argtypes = (HMODULE, LPCSTR)

# https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-loadlibraryexw
LoadLibraryExW = kernel32.LoadLibraryExW
LoadLibraryExW.restype = HMODULE
LoadLibraryExW.argtypes = (LPCWSTR, HANDLE, DWORD)


class WindowsBootstrap(Bootstrap):
    @property
    def name(self) -> str:
        return "WindowsBootstrap"

    @property
    def target_system(self) -> str:
        return "windows"

    @property
    def executable_filename(self) -> str:
        return "bedrock_server.exe"

    @property
    def _endstone_runtime_filename(self) -> str:
        return "endstone_runtime_loader.dll"

    def _add_loopback_exemption(self) -> bool:
        sid = "S-1-15-2-1958404141-86561845-1752920682-3514627264-368642714-62675701-733520436"
        ret = subprocess.run(
            ["CheckNetIsolation", "LoopbackExempt", "-s", f"-p={sid}"], check=True, capture_output=True
        )
        if sid not in str(ret.stdout):
            ret = ctypes.windll.shell32.ShellExecuteW(
                None, "runas", "CheckNetIsolation", " ".join(["LoopbackExempt", "-a", f"-p={sid}"]), None, 1
            )
            return ret > 32
        else:
            return True

    def _create_process(self, *args, **kwargs) -> None:
        self._add_loopback_exemption()

        # Add paths for symbol lookup
        env = os.environ.copy()
        symbol_path = env.get("_NT_SYMBOL_PATH", "")
        symbol_path_list = symbol_path.split(os.pathsep)
        symbol_path_list = [
            str(self._endstone_runtime_path.parent.absolute()),
            str(self.plugin_path.absolute()),
        ] + symbol_path_list
        env["_NT_SYMBOL_PATH"] = os.pathsep.join(symbol_path_list)

        # Create the process is a suspended state
        super()._create_process(creationflags=CREATE_SUSPENDED, env=env)
        handle_proc = int(self._process._handle)
        lib_path = str(self._endstone_runtime_path.absolute())

        # Validate dll
        dll = kernel32.LoadLibraryExW(lib_path, None, DONT_RESOLVE_DLL_REFERENCES)
        if not dll:
            raise ValueError(f"LoadLibraryExW failed with error {get_last_error()}.")

        # Allocate memory for lib_path
        address = kernel32.VirtualAllocEx(
            handle_proc,  # hProcess
            0,  # lpAddress
            MAX_PATH * 2 + 1,  # dwSize
            DWORD(MEM_COMMIT | MEM_RESERVE),  # flAllocationType
            DWORD(PAGE_READWRITE),  # flProtect
        )
        if not address:
            raise ValueError(f"VirtualAllocEx failed with error {get_last_error()}.")

        # Write lib_path into the allocated memory
        size_written = SIZE_T(0)
        result = kernel32.WriteProcessMemory(
            handle_proc, address, lib_path, len(lib_path) * 2 + 1, ctypes.byref(size_written)
        )
        if not result:
            raise ValueError(f"WriteProcessMemory failed with error {get_last_error()}.")

        # Get module handle of kernel32
        handle_kernel32 = kernel32.GetModuleHandleW("kernel32.dll")
        if not handle_kernel32:
            raise ValueError(f"GetModuleHandleW failed with error {get_last_error()}.")

        # Get address of LoadLibraryW
        load_library = kernel32.GetProcAddress(handle_kernel32, b"LoadLibraryW")
        if not load_library:
            raise ValueError(f"GetProcAddress failed with error {get_last_error()}.")

        # Start a new thread in the process, starting at LoadLibraryW with address as argument
        remote_thread = kernel32.CreateRemoteThread(handle_proc, None, 0, load_library, address, 0, None)
        if not remote_thread:
            raise ValueError(f"CreateRemoteThread failed with error {get_last_error()}.")

        # Wait for the remote thread to finish
        if kernel32.WaitForSingleObject(remote_thread, INFINITE) == 0xFFFFFFFF:
            raise ValueError(f"WaitForSingleObject failed with error {get_last_error()}.")

        # Check dll load result
        exit_code = DWORD()
        result = kernel32.GetExitCodeThread(remote_thread, ctypes.byref(exit_code))
        if result == 0:
            raise ValueError(f"LoadLibrary failed with thread exit code: {exit_code.value}")

        # Reopen the handle to process thread (which was closed by subprocess.Popen)
        snapshot = kernel32.CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0)
        if snapshot == INVALID_HANDLE_VALUE:
            raise ValueError(f"CreateToolhelp32Snapshot failed with error {get_last_error()}.")

        thread_entry = THREADENTRY32()
        thread_entry.dwSize = ctypes.sizeof(thread_entry)
        success = kernel32.Thread32First(snapshot, ctypes.byref(thread_entry))
        if not success:
            raise ValueError(f"Thread32First failed with error {get_last_error()}.")

        handle_thread = None
        while success:
            if thread_entry.th32OwnerProcessID == self._process.pid:
                handle_thread = kernel32.OpenThread(THREAD_ALL_ACCESS, None, thread_entry.th32ThreadID)
                break

            success = kernel32.Thread32Next(snapshot, ctypes.byref(thread_entry))

        if handle_thread is None:
            raise ValueError(f"OpenThread failed with error {get_last_error()}.")

        # Resume main thread execution
        kernel32.ResumeThread(handle_thread)
        kernel32.CloseHandle(handle_thread)
