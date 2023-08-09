import re
import sys
from typing import BinaryIO

if sys.version_info >= (3, 11):
    from tomllib import load
else:
    from tomli import load


class PluginDescription:
    VALID_NAME = re.compile(r"^[A-Za-z0-9 _.-]+$")

    def __init__(self, name: str, version: str, main_cls: str):
        if not self.VALID_NAME.match(name):
            raise ValueError(f"Plugin name {name} contains invalid characters.")

        self._name = name.replace(" ", "_")
        self._version = version
        self._main = main_cls
        self._description = None
        self._authors = []
        self._prefix = None

    @property
    def name(self) -> str:
        return self._name

    @property
    def fullname(self) -> str:
        return f"{self._name} v{self._version}"

    @property
    def version(self) -> str:
        return self._version

    @property
    def main(self) -> str:
        return self._main

    @property
    def description(self) -> str:
        return self._description

    @property
    def authors(self) -> list[str]:
        return self._authors

    @property
    def prefix(self) -> str:
        return self._prefix


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
