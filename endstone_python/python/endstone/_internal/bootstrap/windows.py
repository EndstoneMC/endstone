from endstone._internal.bootstrap.base import Bootstrap


class WindowsBootstrap(Bootstrap):
    @property
    def name(self) -> str:
        return "WindowsBootstrap"

    @property
    def target_system(self) -> str:
        return "Windows"

    @property
    def executable_filename(self) -> str:
        return "bedrock_server.exe"

    # async def _create_process(self, *args, **kwargs) -> None:
    #     # https://learn.microsoft.com/en-us/windows/win32/procthread/process-creation-flags
    #     await super()._create_process(creationflags=0x04, *args, **kwargs)  # CREATE_SUSPENDED=0x00000004
