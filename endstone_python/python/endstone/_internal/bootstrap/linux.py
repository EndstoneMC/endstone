import os
import stat

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

    def _download_finished(self) -> None:
        super()._download_finished()
        st = os.stat(self.executable_path)
        os.chmod(self.executable_path, st.st_mode | stat.S_IEXEC)
