import os
import re
import signal
import stat
import subprocess
import sysconfig
from pathlib import Path

from .base import Bootstrap


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
    def _mimalloc_path(self) -> Path:
        return self._endstone_runtime_path.with_name("libmimalloc.so")

    @property
    def _endstone_runtime_env(self) -> dict[str, str]:
        env = super()._endstone_runtime_env
        preload = [p for p in re.split(r"[ :]", env.get("LD_PRELOAD", "")) if p]
        names = [Path(p).name for p in preload]
        # a preloaded allocator, or libc.so.6 for glibc, replaces the bundled one
        if not any("malloc" in n or n.startswith("libc.so") for n in names):
            preload.append(str(self._mimalloc_path))
        preload.append(str(self._endstone_runtime_path))
        env["LD_PRELOAD"] = ":".join(preload)
        env["LD_LIBRARY_PATH"] = str(sysconfig.get_config_var("LIBDIR"))
        return env

    def _prepare(self) -> None:
        super()._prepare()
        st = os.stat(self.executable_path)
        os.chmod(self.executable_path, st.st_mode | stat.S_IEXEC)

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
        # let the server process handle Ctrl+C, then relay its exit code
        signal.signal(signal.SIGINT, signal.SIG_IGN)
        return process.wait()
