import ctypes
from ctypes import get_last_error
from ctypes.wintypes import (
    BOOL,
    DWORD,
    LPCSTR,
    LPCWSTR,
    LPVOID,
    LPCVOID,
    LPDWORD,
    HANDLE,
    HMODULE,
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

# https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getmodulehandlew
GetModuleHandle = kernel32.GetModuleHandleW
GetModuleHandle.restype = HMODULE
GetModuleHandle.argtypes = (LPCWSTR,)

# https://learn.microsoft.com/en-us/windows/win32/api/libloaderapi/nf-libloaderapi-getprocaddress
GetProcAddress = kernel32.GetProcAddress
GetProcAddress.restype = LPVOID
GetProcAddress.argtypes = (HMODULE, LPCSTR)


class WindowsBootstrap(Bootstrap):
    @property
    def name(self) -> str:
        return "WindowsBootstrap"

    @property
    def target_system(self) -> str:
        return "Windows"

    @property
    def executable_filename(self) -> str:
        return "bedrock_server.exe"

    @property
    def _endstone_runtime_filename(self) -> str:
        return "endstone_runtime.dll"

    def _create_process(self, *args, **kwargs) -> None:
        # Create the process is a suspended state
        super()._create_process(creationflags=CREATE_SUSPENDED)
        handle_proc = int(self._process._handle)
        lib_path = str(self._endstone_runtime_path.absolute())

        # Allocate memory for lib_path
        address = kernel32.VirtualAllocEx(
            handle_proc,  # hProcess
            0,  # lpAddress
            MAX_PATH * 2 + 1,  # dwSize
            DWORD(MEM_COMMIT | MEM_RESERVE),  # flAllocationType
            DWORD(PAGE_READWRITE),  # flProtect
        )
        assert address, f"VirtualAllocEx failed with error {get_last_error()}."

        # Write lib_path into the allocated memory
        size_written = SIZE_T(0)
        result = kernel32.WriteProcessMemory(
            handle_proc, address, lib_path, len(lib_path) * 2 + 1, ctypes.byref(size_written)
        )
        assert result, f"WriteProcessMemory failed with error {get_last_error()}."

        # Get module handle of kernel32
        handle_kernel32 = kernel32.GetModuleHandleW("kernel32.dll")
        assert handle_kernel32, f"GetModuleHandleW failed with error {get_last_error()}."

        # Get address of LoadLibraryW
        load_library = kernel32.GetProcAddress(handle_kernel32, b"LoadLibraryW")
        assert load_library, f"GetProcAddress failed with error {get_last_error()}."

        # Start a new thread in the process, starting at LoadLibraryW with address as argument
        remote_thread = kernel32.CreateRemoteThread(handle_proc, None, 0, load_library, address, 0, None)
        assert remote_thread, f"CreateRemoteThread failed with error {get_last_error()}."

        # Wait for the remote thread to finish
        assert (
            kernel32.WaitForSingleObject(remote_thread, INFINITE) != 0xFFFFFFFF
        ), f"WaitForSingleObject failed with error {get_last_error()}."

        # Reopen the handle to process thread (which was closed by subprocess.Popen)
        snapshot = kernel32.CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0)
        assert snapshot != INVALID_HANDLE_VALUE, f"CreateToolhelp32Snapshot failed with error {get_last_error()}."

        thread_entry = THREADENTRY32()
        thread_entry.dwSize = ctypes.sizeof(thread_entry)
        success = kernel32.Thread32First(snapshot, ctypes.byref(thread_entry))
        assert success, f"Thread32First failed with error {get_last_error()}."

        handle_thread = None
        while success:
            if thread_entry.th32OwnerProcessID == self._process.pid:
                handle_thread = kernel32.OpenThread(THREAD_ALL_ACCESS, None, thread_entry.th32ThreadID)
                break

            success = kernel32.Thread32Next(snapshot, ctypes.byref(thread_entry))

        assert handle_thread is not None, f"OpenThread failed with error {get_last_error()}."

        # Resume main thread execution
        kernel32.ResumeThread(handle_thread)
        kernel32.CloseHandle(handle_thread)
