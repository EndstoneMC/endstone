import os
import signal
import stat
import subprocess
import sysconfig

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
    def _endstone_runtime_env(self) -> dict[str, str]:
        env = super()._endstone_runtime_env
        env["LD_PRELOAD"] = str(self._endstone_runtime_path.absolute())
        env["LD_LIBRARY_PATH"] = str(sysconfig.get_config_var("LIBDIR"))
        return env

    def _prepare(self) -> None:
        super()._prepare()
        st = os.stat(self.executable_path)
        os.chmod(self.executable_path, st.st_mode | stat.S_IEXEC)
        os.chmod(self.server_path / "crashpad_handler", st.st_mode | stat.S_IEXEC)

    def _run(self, *args, **kwargs) -> int:  # type: ignore[no-untyped-def]
        self._process = subprocess.Popen(  # type: ignore[call-overload]
            [str(self.executable_path.absolute())],
            text=True,
            encoding="utf-8",
            cwd=str(self.server_path.absolute()),
            env=self._endstone_runtime_env,
            *args,
            **kwargs,
        )

        # Forward termination signals to the Bedrock server so it can shut down
        # gracefully and save the world (e.g. on `docker stop`).
        def _forward(signum, _frame):  # type: ignore[no-untyped-def]
            try:
                self._process.send_signal(signum)
            except ProcessLookupError:
                pass

        for sig in (signal.SIGTERM, signal.SIGINT):
            signal.signal(sig, _forward)

        return int(self._process.wait())
