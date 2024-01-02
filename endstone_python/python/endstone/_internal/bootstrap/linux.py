import os
import stat
from typing import Union

from endstone._internal.bootstrap.base import Bootstrap


class LinuxBootstrap(Bootstrap):
    @property
    def name(self) -> str:
        return "LinuxBootstrap"

    @property
    def target_system(self) -> str:
        return "Linux"

    @property
    def executable_filename(self) -> str:
        return "bedrock_server"

    def download(self, dst: Union[str, os.PathLike], url: str, sha256: str) -> None:
        super().download(dst, url, sha256)
        st = os.stat(self.executable_path)
        os.chmod(self.executable_path, st.st_mode | stat.S_IEXEC)
