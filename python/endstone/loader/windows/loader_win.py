import importlib.resources
import logging
import os.path
import site
from ctypes import get_last_error

from endstone.loader.loader import AbstractLoader
from endstone.loader.windows.win_internal import *

# Logger
logger = logging.getLogger("loader-win")


class WindowsLoader(AbstractLoader):
    def __init__(self, exec_path):
        super().__init__(exec_path)
        self.process = None

    def _find_lib(self) -> str:
        lib_filename = "libendstone.dll"

        # try to find under site-packages folder
        for parent_dir in site.getsitepackages():
            lib_path = os.path.join(parent_dir, "endstone", lib_filename)
            if os.path.exists(lib_path):
                return lib_path

        # try to find under resources path
        with importlib.resources.path("endstone", lib_filename) as lib_path:
            if os.path.exists(lib_path):
                return str(lib_path)

        return lib_filename

    def _create_process(self):
        startupinfo = STARTUPINFO()
        startupinfo.cb = ctypes.sizeof(startupinfo)
        process_information = PROCESS_INFORMATION()

        result = CreateProcess(
            None,  # lpApplicationName
            self.exec_path,  # lpCommandLine
            None,  # lpProcessAttributes
            None,  # lpThreadAttributes
            False,  # bInheritHandles
            CREATE_SUSPENDED,  # dwCreationFlags
            None,  # lpEnvironment
            None,  # lpCurrentDirectory
            ctypes.byref(startupinfo),  # lpStartupInfo
            ctypes.byref(process_information),  # lpProcessInformation
        )

        if result:
            self.process = process_information

        return result

    def _allocate_memory(self, size):
        return VirtualAllocEx(
            self.process.hProcess,  # hProcess
            0,  # lpAddress
            size,  # dwSize
            DWORD(MEM_COMMIT),  # flAllocationType
            DWORD(PAGE_READWRITE),  # flProtect
        )

    def _prepare_lib(self, address):
        size_written = SIZE_T(0)
        return WriteProcessMemory(
            self.process.hProcess, address, self.lib_path, len(self.lib_path) * 2 + 1, ctypes.byref(size_written)
        )

    def _load_lib(self, address):
        handle_kernel32 = GetModuleHandle("kernel32.dll")
        if not handle_kernel32:
            logger.error(f"GetModuleHandleW failed with error {get_last_error()}.")
            exit(1)

        load_library = GetProcAddress(handle_kernel32, b"LoadLibraryW")

        if not load_library:
            logger.error(f"GetProcAddress failed with error {get_last_error()}.")
            exit(1)

        # Start a new thread in the process, starting at LoadLibraryW with address as argument
        remote_thread = CreateRemoteThread(self.process.hProcess, None, 0, load_library, address, 0, None)

        return remote_thread

    def start(self):
        if not self._create_process():
            logger.error(f"CreateProcess failed with error {get_last_error()}.")
            self.stop(-1)

        address = self._allocate_memory(MAX_PATH * 2 + 1)
        if not address:
            logger.error(f"VirtualAllocEx failed with error {get_last_error()}.")
            self.stop(-1)

        if not self._prepare_lib(address):
            logger.error(f"WriteProcessMemory failed with error {get_last_error()}.")
            self.stop(-1)

        remote_thread = self._load_lib(address)
        if not remote_thread:
            logger.error(f"CreateRemoteThread failed with error {get_last_error()}.")
            self.stop(-1)

        # Wait for the remote thread to finish
        WaitForSingleObject(remote_thread, INFINITE)

        # Resume main thread execution
        ResumeThread(self.process.hThread)

        # Wait for the main thread to finish
        WaitForSingleObject(self.process.hProcess, INFINITE)

    def stop(self, code: int):
        if self.process:
            TerminateProcess(self.process.hProcess, code)

        exit(code)
