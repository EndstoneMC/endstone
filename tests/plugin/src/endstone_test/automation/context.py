from __future__ import annotations

from dataclasses import dataclass
from math import floor

from endstone import Server
from endstone.command import CommandSenderWrapper
from endstone.level import Location
from endstone.level import Dimension
from endstone.plugin import Plugin


@dataclass
class AutomationContext:
    """Explicit dependencies shared by automated event scenario builders."""

    plugin: Plugin
    server: Server
    bot_name: str
    run_number: int
    selected_scenario_names: set[str] | None

    @classmethod
    def from_runner(cls, runner) -> "AutomationContext":
        return cls(
            plugin=runner.plugin,
            server=runner.server,
            bot_name=runner._bot_name,
            run_number=runner._run_number,
            selected_scenario_names=runner._selected_scenario_names,
        )

    @property
    def overworld(self):
        dimension = self.server.level.get_dimension(Dimension.OVERWORLD)
        if dimension is None:
            raise RuntimeError("The overworld is not available")
        return dimension

    @property
    def quiet_command_sender(self) -> CommandSenderWrapper:
        return CommandSenderWrapper(self.server.command_sender)

    @property
    def tag_prefix(self) -> str:
        return f"endstone_auto_{id(self):x}_{self.run_number}"

    def dispatch_command(self, command_line: str) -> bool:
        return self.server.dispatch_command(self.quiet_command_sender, command_line)

    def command(self, command_line: str) -> None:
        if not self.dispatch_command(command_line):
            raise RuntimeError(f"Command failed: {command_line}")

    def set_block_type(
        self, x: int, y: int, z: int, block_type: str, *, apply_physics: bool = False
    ) -> None:
        """Set a fixture block without logging a failed no-op command."""
        block = self.overworld.get_block_at(x, y, z)
        if str(block.type) != block_type:
            block.set_type(block_type, apply_physics=apply_physics)

    def set_region_type(
        self,
        x1: int, y1: int, z1: int,
        x2: int, y2: int, z2: int,
        block_type: str,
    ) -> None:
        for x in range(min(x1, x2), max(x1, x2) + 1):
            for y in range(min(y1, y2), max(y1, y2) + 1):
                for z in range(min(z1, z2), max(z1, z2) + 1):
                    self.set_block_type(x, y, z, block_type)

    def remove_actors(
        self,
        actor_type: str | None = None,
        *,
        tag: str | None = None,
        center: Location | None = None,
        radius: float | None = None,
    ) -> None:
        """Remove only existing fixture actors; never issue an empty selector."""
        radius_squared = radius * radius if radius is not None else None
        for actor in tuple(self.overworld.actors):
            if not actor.is_valid:
                continue
            if actor_type is not None and str(actor.type) != actor_type:
                continue
            if tag is not None and tag not in actor.scoreboard_tags:
                continue
            if center is not None and radius_squared is not None:
                if actor.location.distance_squared(center) > radius_squared:
                    continue
            actor.remove()

    def clear_player_inventory(self) -> None:
        player = self.server.get_player(self.bot_name)
        if player is not None and any(
            item is not None and item.amount > 0 for item in player.inventory.contents
        ):
            player.inventory.clear()

    def give_player_item(self, item_type: str, count: int = 1) -> None:
        self.clear_player_inventory()
        self.command(f"give {self.bot_name} {item_type} {count}")

    def run_gametest(self, case: str) -> None:
        if not self.dispatch_command(f"gametest run endstone_events:{case}"):
            raise RuntimeError(f"GameTest command failed for {case!r}")

    def auto_tag(self, name: str) -> str:
        return f"{self.tag_prefix}_{name}"

    def player_for_action(self):
        player = self.server.get_player(self.bot_name)
        if player is None:
            raise RuntimeError(f"player driver {self.bot_name!r} is not online")
        return player

    def block_target(self, offset: tuple[int, int, int] = (1, 0, 0)) -> tuple[int, int, int]:
        location = self.player_for_action().location
        return (
            floor(location.x) + offset[0],
            floor(location.y) + offset[1],
            floor(location.z) + offset[2],
        )
