import sys
from typing import BinaryIO, Optional

from endstone._bindings import PluginDescription

from endstone.command import Command

if sys.version_info >= (3, 11):
    from tomllib import load
else:
    from tomli import load


class PluginDescriptionFile(PluginDescription):
    def __init__(self, fp: BinaryIO):
        data = load(fp)

        try:
            name = data.pop("name")
            version = data.pop("version")
            main = data.pop("main")
            PluginDescription.__init__(self, name, version)
            self._main: str = main
            self._description: Optional[str] = None
            self._authors: Optional[list[str]] = None
            self._prefix: Optional[str] = None
            self._commands: Optional[dict] = None

        except KeyError as e:
            raise RuntimeError(f"{e} is not defined")

        for k, v in data.items():
            if hasattr(self, f"_{k}"):
                setattr(self, f"_{k}", v)

    @property
    def main(self) -> str:
        return self._main

    @property
    def description(self) -> Optional[str]:
        return self._description

    @property
    def authors(self) -> Optional[list[str]]:
        return self._authors

    @property
    def prefix(self) -> Optional[str]:
        return self._prefix

    @property
    def commands(self) -> list[Command]:
        # TODO: implement this
        return []

    def _get_description(self):
        return self.description

    def _get_authors(self):
        return self.authors

    def _get_prefix(self):
        return self.prefix

    def _get_commands(self):
        return self.commands
