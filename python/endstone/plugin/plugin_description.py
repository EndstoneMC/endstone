import re
import sys
from typing import BinaryIO

from endstone._plugin import IPluginDescription

if sys.version_info >= (3, 11):
    from tomllib import load
else:
    from tomli import load


class PluginDescription(IPluginDescription):
    VALID_NAME = re.compile(r"^[A-Za-z0-9 _.-]+$")

    def __init__(self, name: str, version: str, main_cls: str):
        IPluginDescription.__init__(self)

        if not self.VALID_NAME.match(name):
            raise ValueError(f"Plugin name {name} contains invalid characters.")

        self._name = name.replace(" ", "_")
        self._version = version
        self._main = main_cls
        self._description = None
        self._authors = []
        self._prefix = None

    def get_name(self):
        return self._name

    def get_version(self) -> str:
        return self._version

    def get_main(self) -> str:
        return self._main

    def get_description(self) -> str:
        return self._description

    def get_authors(self) -> list[str]:
        return self._authors

    def get_prefix(self) -> str:
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
