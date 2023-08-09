import ctypes
import platform
from ctypes.wintypes import *

SIZE_T = ctypes.c_size_t
PSIZE_T = ctypes.POINTER(SIZE_T)

# Platform check
assert platform.system() == "Windows"


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


class PROCESS_INFORMATION(ctypes.Structure):
    _fields_ = [
        ("hProcess", HMODULE),
        ("hThread", HMODULE),
        ("dwProcessId", DWORD),
        ("dwThreadId", DWORD),
    ]


LPPROCESS_INFORMATION = ctypes.POINTER(PROCESS_INFORMATION)

# Constants
CREATE_SUSPENDED = 0x00000004
MAX_PATH = 260
MEM_COMMIT = 0x00001000 | 0x2000
PAGE_READWRITE = 0x04
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
