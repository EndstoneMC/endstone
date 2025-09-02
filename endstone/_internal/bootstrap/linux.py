import ctypes.util
import os
import stat
import subprocess
from pathlib import Path

from endstone._internal.bootstrap.base import Bootstrap


class LinuxBootstrap(Bootstrap):
    @property
    def name(self) -> str:
        return "LinuxBootstrap"

    @property
    def target_system(self) -> str:
        return "linux"

    @property
    def executable_filename(self) -> str:
        return "bedrock_server"

    @property
    def _endstone_runtime_filename(self) -> str:
        return "libendstone_runtime.so"

    @property
    def _endstone_runtime_env(self) -> dict[str, str]:
        env = super()._endstone_runtime_env
        env["LD_PRELOAD"] = str(self._endstone_runtime_path.absolute())
        env["LD_LIBRARY_PATH"] = str(self._linked_libpython_path.parent.absolute())
        return env

    def _prepare(self) -> None:
        super()._prepare()
        st = os.stat(self.executable_path)
        os.chmod(self.executable_path, st.st_mode | stat.S_IEXEC)
        os.chmod(self.server_path / "crashpad_handler", st.st_mode | stat.S_IEXEC)

    def _run(self, *args, **kwargs) -> int:
        process = subprocess.Popen(
            [str(self.executable_path.absolute())],
            text=True,
            encoding="utf-8",
            cwd=str(self.server_path.absolute()),
            env=self._endstone_runtime_env,
            *args,
            **kwargs,
        )
        return process.wait()

    @property
    def _linked_libpython_path(self) -> Path:
        """
        Find the path of the linked libpython on Unix systems.

        From https://gist.github.com/tkf/d980eee120611604c0b9b5fef5b8dae6

        Returns:
            (Path): Path object representing the path of the linked libpython.
        """

        class DlInfo(ctypes.Structure):
            # https://www.man7.org/linux/man-pages/man3/dladdr.3.html
            _fields_ = [
                ("dli_fname", ctypes.c_char_p),
                ("dli_fbase", ctypes.c_void_p),
                ("dli_sname", ctypes.c_char_p),
                ("dli_saddr", ctypes.c_void_p),
            ]

        libdl = ctypes.CDLL(ctypes.util.find_library("dl"))
        libdl.dladdr.argtypes = [ctypes.c_void_p, ctypes.POINTER(DlInfo)]
        libdl.dladdr.restype = ctypes.c_int

        dlinfo = DlInfo()
        retcode = libdl.dladdr(ctypes.cast(ctypes.pythonapi.Py_GetVersion, ctypes.c_void_p), ctypes.pointer(dlinfo))
        if retcode == 0:
            raise ValueError("dladdr cannot match the address of ctypes.pythonapi.Py_GetVersion to a shared object")

        path = Path(dlinfo.dli_fname.decode()).resolve()
        return path
