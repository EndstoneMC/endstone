import sys
from typing import BinaryIO

from endstone.plugin import PluginDescription

if sys.version_info >= (3, 11):
    from tomllib import load
else:
    from tomli import load


class PluginDescriptionFile(PluginDescription):
    def __init__(self, fp: BinaryIO) -> None:
        data = load(fp)

        for prop in ["name", "version", "main"]:
            assert prop in data, f"Invalid plugin.toml: property {prop} is not defined"

        name = data["name"]
        assert (
            name is not None and len(name) > 0 and len(name.split(":")) == 2
        ), "Invalid main: must be {module_name}:{class_name}"

        PluginDescription.__init__(self, **data)
