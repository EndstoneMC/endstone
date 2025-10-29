import _winapi
import ctypes
import os
import subprocess
import sys
import warnings
from subprocess import STARTUPINFO, Handle, list2cmdline

from endstone._internal import _detours
from endstone._internal.bootstrap.base import Bootstrap


class PopenWithDll(subprocess.Popen):
    def __init__(self, *args, **kwargs):
        self.dll_names = kwargs.pop("dll_names", None)
        super().__init__(*args, **kwargs)

    def _execute_child(
        self,
        args,
        executable,
        preexec_fn,
        close_fds,
        pass_fds,
        cwd,
        env,
        startupinfo,
        creationflags,
        shell,
        p2cread,
        p2cwrite,
        c2pread,
        c2pwrite,
        errread,
        errwrite,
        # unused_restore_signals,
        # unused_gid,
        # unused_gids,
        # unused_uid,
        # unused_umask,
        # unused_start_new_session,
        # unused_process_group,
        *unused_args,
        **unused_kwargs,
    ):
        """Execute program (MS Windows version)"""

        assert not pass_fds, "pass_fds not supported on Windows."

        if isinstance(args, str):
            pass
        elif isinstance(args, bytes):
            if shell:
                raise TypeError("bytes args is not allowed on Windows")
            args = list2cmdline([args])
        elif isinstance(args, os.PathLike):
            if shell:
                raise TypeError("path-like args is not allowed when shell is true")
            args = list2cmdline([args])
        else:
            args = list2cmdline(args)

        if executable is not None:
            executable = os.fsdecode(executable)

        if startupinfo is None:
            startupinfo = STARTUPINFO()
        else:
            startupinfo = startupinfo.copy()

        use_std_handles = -1 not in (p2cread, c2pwrite, errwrite)
        if use_std_handles:
            startupinfo.dwFlags |= _winapi.STARTF_USESTDHANDLES
            startupinfo.hStdInput = p2cread
            startupinfo.hStdOutput = c2pwrite
            startupinfo.hStdError = errwrite

        attribute_list = startupinfo.lpAttributeList
        have_handle_list = bool(attribute_list and "handle_list" in attribute_list and attribute_list["handle_list"])

        # If we were given an handle_list or need to create one
        if have_handle_list or (use_std_handles and close_fds):
            if attribute_list is None:
                attribute_list = startupinfo.lpAttributeList = {}
            handle_list = attribute_list["handle_list"] = list(attribute_list.get("handle_list", []))

            if use_std_handles:
                handle_list += [int(p2cread), int(c2pwrite), int(errwrite)]

            handle_list[:] = self._filter_handle_list(handle_list)

            if handle_list:
                if not close_fds:
                    warnings.warn("startupinfo.lpAttributeList['handle_list'] overriding close_fds", RuntimeWarning)

                # When using the handle_list we always request to inherit
                # handles but the only handles that will be inherited are
                # the ones in the handle_list
                close_fds = False

        assert not shell

        if cwd is not None:
            cwd = os.fsdecode(cwd)

        # Start the process
        try:
            hp, ht, pid, tid = _detours.CreateProcessWithDllEx(
                executable,
                args,
                # no special security
                None,
                None,
                not close_fds,
                creationflags,
                env,
                cwd,
                startupinfo,
                dll_name=self.dll_names,
            )
        finally:
            self._close_pipe_fds(p2cread, p2cwrite, c2pread, c2pwrite, errread, errwrite)

        # Retain the process handle, but close the thread handle
        self._child_created = True
        self._handle = Handle(hp)
        self.pid = pid
        _winapi.CloseHandle(ht)


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

    @property
    def _endstone_runtime_env(self) -> dict[str, str]:
        env = super()._endstone_runtime_env
        symbol_path = env.get("_NT_SYMBOL_PATH", "")
        symbol_path_list = symbol_path.split(os.pathsep)
        symbol_path_list = [
            str(self._endstone_runtime_path.parent.absolute()),
            str(self.plugin_path.absolute()),
        ] + symbol_path_list
        env["_NT_SYMBOL_PATH"] = os.pathsep.join(symbol_path_list)
        return env

    def _add_loopback_exemption(self) -> None:
        sid = "S-1-15-2-1958404141-86561845-1752920682-3514627264-368642714-62675701-733520436"
        ret = subprocess.run(
            ["CheckNetIsolation", "LoopbackExempt", "-s", f"-p={sid}"], check=True, capture_output=True
        )
        if sid not in str(ret.stdout):
            ret = ctypes.windll.shell32.ShellExecuteW(
                None, "runas", "CheckNetIsolation", " ".join(["LoopbackExempt", "-a", f"-p={sid}"]), None, 1
            )
            if ret <= 32:
                raise RuntimeError(f"CheckNetIsolation LoopbackExempt -a failed with exit code {ret}.")

    def _check_python_distribution(self) -> None:
        if "WindowsApp" in sys.base_prefix:
            self._logger.warning("You are using a Microsoft Store distribution of Python which is not supported.")
            self._logger.warning(
                "If you experience issues, please use a regular Python distribution instead "
                "from https://www.python.org/downloads/ ."
            )

    def _run(self, *args, **kwargs) -> int:
        try:
            self._add_loopback_exemption()
        except Exception as e:
            self._logger.warning(
                f"Unable to add loopback exemption: %s. See bedrock_server_how_to.html for more details. {e}"
            )

        self._check_python_distribution()
        process = PopenWithDll(
            [str(self.executable_path.absolute())],
            text=True,
            encoding="utf-8",
            cwd=str(self.server_path.absolute()),
            env=self._endstone_runtime_env,
            dll_names=str(self._endstone_runtime_path.absolute()),
            *args,
            **kwargs,
        )
        return process.wait()
