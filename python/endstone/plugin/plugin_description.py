import sys
from typing import BinaryIO, Optional

from endstone._bindings import PluginDescription

from endstone.command import Command

if sys.version_info >= (3, 11):
    # noinspection PyUnresolvedReferences
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
        except KeyError as e:
            raise RuntimeError(f"{e} is not defined")

        self._main: str = main
        self._description: Optional[str] = None
        self._authors: Optional[list[str]] = None
        self._prefix: Optional[str] = None
        self._commands: list[Command] = []

        for k, v in data.items():
            if k == "commands":
                self._commands = self._parse_command_map(v)
                continue

            if hasattr(self, f"_{k}"):
                setattr(self, f"_{k}", v)

    def _parse_command_map(self, command_map: dict) -> list[Command]:
        commands = []
        for name, value in command_map.items():
            if ":" in name:
                raise NameError(f"Could not load command {name} for plugin {self.name}: Illegal Characters")

            command = Command(name)

            description = value.get("description", None)
            usages = value.get("usages", None)
            aliases = value.get("aliases", None)

            # TODO: permissions:
            #  permission = value.get("permission", None)
            #  permission_message = value.get("permission-message", None)

            if description:
                command.description = description

            if usages:
                if isinstance(usages, list):
                    usage_list = usages
                else:
                    usage_list = [usages]

                command.usages = usage_list

            if aliases:
                alias_list = []

                if isinstance(aliases, list):
                    for alias in aliases:
                        if ":" in alias:
                            raise NameError(f"Could not load alias {alias} for plugin {self.name}: Illegal Characters")

                        alias_list.append(alias)
                else:
                    if ":" in aliases:
                        raise NameError(f"Could not load alias {aliases} for plugin {self.name}: Illegal Characters")

                    alias_list.append(aliases)

                command.aliases = alias_list

            # TODO: permission system
            # if permission:
            #     command.permission = permission
            # if permission_message:
            #     command.permission_message = permission_message

            commands.append(command)

        return commands

    @property
    def main(self) -> str:
        return self._main

    @property
    def description(self) -> Optional[str]:
        return self._get_description()

    @property
    def authors(self) -> Optional[list[str]]:
        return self._get_authors()

    @property
    def prefix(self) -> Optional[str]:
        return self._get_prefix()

    @property
    def commands(self) -> list[Command]:
        return self._get_commands()

    def _get_description(self) -> Optional[str]:
        return self._description

    def _get_authors(self) -> Optional[list[str]]:
        return self._authors

    def _get_prefix(self) -> Optional[str]:
        return self._prefix

    def _get_commands(self) -> list[Command]:
        return self._commands
