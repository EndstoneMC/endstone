import re
import sys
from typing import BinaryIO, Optional, final

if sys.version_info >= (3, 11):
    from tomllib import load
else:
    from tomli import load


class PluginDescription:
    VALID_NAME = re.compile(r"^[A-Za-z0-9 _.-]+$")

    def __init__(self, name: str, version: str, main_cls: str):
        if not self.VALID_NAME.match(name):
            raise ValueError(f"Plugin name {name} contains invalid characters.")

        self._name: str = name.replace(" ", "_")
        self._version: str = version
        self._main: str = main_cls
        self._description: Optional[str] = None
        self._authors: Optional[list[str]] = []
        self._prefix: Optional[str] = None

    @final
    @property
    def name(self) -> str:
        return self._name

    @final
    @property
    def version(self) -> str:
        return self._version

    @final
    @property
    def main(self) -> str:
        return self._main

    @final
    @property
    def description(self) -> Optional[str]:
        return self._description

    @final
    @property
    def authors(self) -> Optional[list[str]]:
        return self._authors

    @final
    @property
    def prefix(self) -> Optional[str]:
        return self._prefix

    @final
    @property
    def fullname(self) -> str:
        return f"{self.name} v{self.version}"


class PluginDescriptionFile(PluginDescription):
    def __init__(self, fp: BinaryIO):
        data = load(fp)

        try:
            name = data.pop("name")
            version = data.pop("version")
            main = data.pop("main")
            super().__init__(name, version, main)
        except KeyError as e:
            raise RuntimeError(f"{e} is not defined")

        for k, v in data.items():
            if hasattr(self, f"_{k}"):
                setattr(self, f"_{k}", v)
