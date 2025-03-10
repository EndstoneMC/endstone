import ctypes
import os
import subprocess

from endstone._internal.bootstrap.base import Bootstrap
from endstone._internal.winext import start_process_with_dll


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

    def _run(self, *args, **kwargs) -> int:
        self._add_loopback_exemption()

        return start_process_with_dll(
            str(self.executable_path.absolute()),
            str(self._endstone_runtime_path.absolute()),
            cwd=str(self.server_path.absolute()),
            env=self._endstone_runtime_env,
        )
