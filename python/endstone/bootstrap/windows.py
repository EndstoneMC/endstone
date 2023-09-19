import ctypes
import logging
from ctypes import get_last_error
from ctypes.wintypes import (
    BOOL,
    WORD,
    DWORD,
    UINT,
    LPWSTR,
    LPCSTR,
    LPCWSTR,
    LPVOID,
    LPCVOID,
    LPBYTE,
    LPDWORD,
    HANDLE,
    HMODULE,
)
from pathlib import Path
from typing import Union

from . import find_library

logger = logging.getLogger("loader")

SIZE_T = ctypes.c_size_t
PSIZE_T = ctypes.POINTER(SIZE_T)


class STARTUPINFO(ctypes.Structure):
    _fields_ = [
        ("cb", DWORD),
        ("lpReserved", LPWSTR),
        ("lpDesktop", LPWSTR),
        ("lpTitle", LPWSTR),
        ("dwX", DWORD),
        ("dwY", DWORD),
        ("dwXSize", DWORD),
        ("dwYSize", DWORD),
        ("dwXCountChars", DWORD),
        ("dwYCountChars", DWORD),
        ("dwFillAttribute", DWORD),
        ("dwFlags", DWORD),
        ("wShowWindow", WORD),
        ("cbReserved2", WORD),
        ("lpReserved2", LPBYTE),
        ("hStdInput", HANDLE),
        ("hStdOutput", HANDLE),
        ("hStdError", HANDLE),
    ]


LPSTARTUPINFO = ctypes.POINTER(STARTUPINFO)


# noinspection PyPep8Naming
class PROCESS_INFORMATION(ctypes.Structure):
    _fields_ = [
        ("hProcess", HMODULE),
        ("hThread", HMODULE),
        ("dwProcessId", DWORD),
        ("dwThreadId", DWORD),
    ]


LPPROCESS_INFORMATION = ctypes.POINTER(PROCESS_INFORMATION)

# Constants
CREATE_SUSPENDED = 0x00000004  # https://learn.microsoft.com/en-us/windows/win32/procthread/process-creation-flags
MAX_PATH = 260  # https://learn.microsoft.com/en-us/windows/win32/fileio/maximum-file-path-limitation
MEM_COMMIT = 0x00001000  # https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc
MEM_RESERVE = 0x00002000  # https://learn.microsoft.com/en-us/windows/win32/api/memoryapi/nf-memoryapi-virtualalloc
PAGE_READWRITE = 0x04  # https://learn.microsoft.com/en-us/windows/win32/memory/memory-protection-constants
INFINITE = 0xFFFFFFFF

# Kernel32 Functions
kernel32 = ctypes.WinDLL("kernel32", use_last_error=True)

# https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessw
CreateProcess = kernel32.CreateProcessW
CreateProcess.restype = BOOL
CreateProcess.argtypes = (
    LPCWSTR,
    LPWSTR,
    LPVOID,
    LPVOID,
    BOOL,
    DWORD,
    LPVOID,
    LPCWSTR,
    LPSTARTUPINFO,
    LPPROCESS_INFORMATION,
)

# https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-terminateprocess
TerminateProcess = kernel32.TerminateProcess
TerminateProcess.restype = BOOL
TerminateProcess.argtypes = (HANDLE, UINT)

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

# https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject
WaitForSingleObject = kernel32.WaitForSingleObject
WaitForSingleObject.restype = DWORD
WaitForSingleObject.argtypes = (HANDLE, DWORD)

# https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-resumethread
ResumeThread = kernel32.ResumeThread
ResumeThread.restype = DWORD
ResumeThread.argtypes = (HANDLE,)


def start_server(exec_path: Union[str, Path]):
    # Find necessary libraries
    lib_path = str(find_library("libendstone.dll").absolute())

    # Create process
    startup_info = STARTUPINFO()
    startup_info.cb = ctypes.sizeof(startup_info)
    process = PROCESS_INFORMATION()
    result = CreateProcess(
        None,  # lpApplicationName
        str(exec_path),  # lpCommandLine
        None,  # lpProcessAttributes
        None,  # lpThreadAttributes
        False,  # bInheritHandles
        CREATE_SUSPENDED,  # dwCreationFlags
        None,  # lpEnvironment
        None,  # lpCurrentDirectory
        ctypes.byref(startup_info),  # lpStartupInfo
        ctypes.byref(process),  # lpProcessInformation
    )
    assert result, f"CreateProcess failed with error {get_last_error()}."

    # Allocate memory for lib_path
    address = VirtualAllocEx(
        process.hProcess,  # hProcess
        0,  # lpAddress
        MAX_PATH * 2 + 1,  # dwSize
        DWORD(MEM_COMMIT | MEM_RESERVE),  # flAllocationType
        DWORD(PAGE_READWRITE),  # flProtect
    )
    assert address, f"VirtualAllocEx failed with error {get_last_error()}."

    # Write lib_path into the allocated memory
    size_written = SIZE_T(0)
    result = WriteProcessMemory(process.hProcess, address, lib_path, len(lib_path) * 2 + 1, ctypes.byref(size_written))
    assert result, f"WriteProcessMemory failed with error {get_last_error()}."

    # Get module handle of kernel32
    hk32 = GetModuleHandle("kernel32.dll")
    assert hk32, f"GetModuleHandleW failed with error {get_last_error()}."

    # Get address of LoadLibraryW
    load_library = GetProcAddress(hk32, b"LoadLibraryW")
    assert load_library, f"GetProcAddress failed with error {get_last_error()}."

    # Start a new thread in the process, starting at LoadLibraryW with address as argument
    remote_thread = CreateRemoteThread(process.hProcess, None, 0, load_library, address, 0, None)
    assert remote_thread, f"CreateRemoteThread failed with error {get_last_error()}."

    # Wait for the remote thread to finish
    WaitForSingleObject(remote_thread, INFINITE)

    # Resume main thread execution
    ResumeThread(process.hThread)

    # Wait for the main thread to finish
    WaitForSingleObject(process.hProcess, INFINITE)
